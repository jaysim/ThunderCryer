/**
 ******************************************************************************
 * @file        CDCF77.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        19.08.2013
 * @project 	ThunderCryer
 *
 * ported from http://www.gjlay.de/software/c-code/dcf77.html
 ******************************************************************************
 * @example
 * 	// Alle 10 Millisekunden aus einer ISR heraus aufrufen
 *	// Macht alle 10ms einen Schnappschuss vom DCF-Port (hier an Port B1).
 *	void job_dcf77_10ms (void)
 *	{
 *		uint8_t bit = 0;
 *
 *		if (PINB & (1 << PB1))
 *			bit = 1;
 *
 *		dcf77_bit = bit;
 *	}
 *
 ******************************************************************************
 * @example
 *	// DCF-Abgleich erfolgreich?
 *	if (dcf77.time_changed)
 *	{
 *		// Neue Zeit ist in time angekommen!
 *		// Merker zurücksetzen
 *		dcf77.time_changed = 0;
 *
 *		// Zeit in dcf77.ptime auswerten
 *		// ...
 *	}
 *
 ******************************************************************************
 */

#include "CDCF77.h"


#define PARITY 0xff
#define IGNORE 0xfe
#define NEWVAL 0x10

#define X(N) ((uint8_t)(&newtime.N - &newtime.minute))


#define X0 X(minute)
#define X1 X(hour)
#define X2 X(day)
#define X3 X(day_of_week)
#define X4 X(month)
#define X5 X(year)
#define X6 X(mesz)


CDCF77::CDCF77():dcf_byteno{
  #ifdef TIME_MESZ
   // Zeitzone: MEZ/MESZ (Winter-/Sommerzeit): 2 Bits an Offset 6
   NEWVAL | X6, X6,
   // Ankündigung Schaltsekunde
   IGNORE,
   // Parity Zeitzone (immer 1)
   PARITY,
  #endif // TIME_MESZ

   // Minute: 7 Bits an Offset 0
   NEWVAL | X0, X0, X0, X0, X0, X0, X0,
   // Parity Minute
   PARITY,
   // Stunde: 6 Bits an Offset 1
   NEWVAL | X1, X1, X1, X1, X1, X1,
   // Parity Stunde
   PARITY,
   // Tag: 6 Bits an Offset 2
   NEWVAL | X2, X2, X2, X2, X2, X2,
   // Wochentag: 3 Bits an Offset 3
   NEWVAL | X3, X3, X3,
   // Monat: 5 Bits an Offset 4
   NEWVAL | X4, X4, X4, X4, X4,
   // Jahr: 8 Bits an Offset 5
   NEWVAL | X5, X5, X5, X5, X5, X5, X5, X5,
   // Parity Datum
   PARITY
  }
{

}

CDCF77::~CDCF77() {
	// TODO Auto-generated destructor stub
}




/*
   Dies ist der Einstiegspunkt für das DCF-Modul. Die Funktion wird alle
   10ms (ein Tick) aufgerufen.

   poll = 0: Träger ist abgesenkt (erste 100ms oder 200ms einer Sekunde)
   poll = 1: Voller Träger */

void CDCF77::Tick (uint8_t poll)
{
    // Diese Funktion wird zu jedem Tick -- also alle 10ms --
    // aufgerufen. Ticks um 1 weiterzählen.

    uint8_t ticks = 1 + this->ticks;

    // Überlauf der Ticks, z.B. bei schlechtem Signal oder nicht-
    // angeschlossenem Empfänger. Ticks bleiben dann bei 255 stehen.

    if (ticks == 0)
        ticks = UINT8_MAX;

    this->ticks = ticks;

    // poll normieren auf 0 oder !0
    poll = !!poll;

    // Flanke?
    if (poll != this->poll)
    {
        // Ja. poll-Wert zur Erkennung der nächsten Flanke merken.
        this->poll = poll;

        if (poll)
        {
            // Flanke 0->1: Bit-Ende 100ms oder 200ms nach Sekundenstart
        	CDCF77::EndBit (ticks);
        }
        else
        {
            // Flanke 1->0: Bit-Anfang, Sekundenstart
            CDCF77::StartBit (ticks);
            this->ticks = 0;
        }
    }
}


/* Wird aufgerufen bei Flanke 1->0 des DCF-Signals.
   Start eines neuen Bits bzw. Start/Ende einer Sekunde.
   TICKS sind die Anzahl Ticks seit der vorherigen 1->0 Flanke
   und ist ca. 100 (also etwa 1s) oder nach dem ausgelassenen
   59. Bit ca. 200 (also etwa 2s). */

void CDCF77::StartBit (uint8_t ticks)
{
    // Aktuelle Sekunde 0..59 um 1 weiterzählen
    uint8_t second = this->newtime.second;

    if (second < 60)
        second++;

    this->newtime.second = second;

    if (ticks > 200-10 && ticks < 200+10)
    {
        // 59. Sekunde: Der Träger wurde für ca. 200 Ticks (2s)
        // nicht abgesenkt. Nun beginnt eine neue Absenkung, d.h.
        // eine neue Minute beginnt.

        this->newtime.second = 0;

        if (this->error == 0 && second == 59)
        {
            // Während einer ganzen Minute wurden korrekte
            // Bits empfangen --> aktuelle Zeit/Datum speichern

            this->time_changed = 1;

            // !!! FÜR .ptime SIEHE DEN KOMMENTAR IN dcf77.h     !!!

            if (this->ptime)
                *(this->ptime) = this->newtime;
        }

        // Eine Minuten-Marke bewirkt ein Neubeginn der
        // DCF-Auswertung; daher das Fehler-Flag zurücksetzen.

        this->error = 0;
    }
    else
    {
        // Eine "normale" Sekunde ist vergangen. Deren Länge
        // ist ca. 100 Ticks (1s). Andernfalls ist's ein Fehler.

        if (ticks < 100-10 || ticks > 100+10)
            this->error = 1;
    }
}


/* Wird aufgerufen bei Flanke 0->1 des DCF-Signals.
   Ende der Trägerabsenkung, d.h. Bitende. TICKS ist
   die Dauer der Trägerabsenkung, d.h. die Anzahl der
   10ms-Ticks seit der letzten 1->0 Flanke des DCF-Signals
   und ist für gültige 0-Bits ca. 10 (100ms) und für
   gültige 1-Bits ca. 20 (200ms). */

void CDCF77::EndBit (uint8_t ticks)
{
    uint8_t bit = 0;

    if (ticks >= 10-3 && ticks < 10+3)
        // Ein 0-Bit dauert ca. 10 Ticks (100ms)
        bit = 0;
    else if (ticks >= 20-3 && ticks < 20+3)
        // Ein 1-Bit dauert ca. 20 Ticks (200ms)
        bit = 1;
    else
        // Alle anderen Bitlängen sind ungültig --> Fehler
        this->error = 1;

    if (!this->error)
        StoreBit(bit);
}


/* Speichert BIT in der DCF-Struktur */

void CDCF77::StoreBit (uint8_t bit)
{
    // Nur Bits 17..58 bzw. 21..58 decodieren. Die aktuelle Sekunde,
    // vermindert um 17/21, dient als Index ins Feld dcf_byteno[],
    // wo Informationen dazu stehen, wie BIT zu
    // behandeln und wo es ggf. zu speichern ist.

    const uint8_t nbits = sizeof (dcf_byteno) / sizeof (dcf_byteno[0]);

    uint8_t i = this->newtime.second - (59-nbits);

    if (i >= nbits)
        return;

    i = dcf_byteno[i];

#ifdef TIME_MESZ
    if (i == IGNORE)
        return;
#endif // TIME_MESZ

    this->parity ^= bit;

    if (i == PARITY)
    {
        // Partity-Test. Diese Funktion wird nur aufgerufen mit
        // .error = 0. Falls parity != 0 ist, liegt also ein
        // Parity-Fehler vor. Ansonsten bleibt .error = 0 unverändert.
        this->error = this->parity;

        // Parity für nächstes Bit zurücksetzen.
        this->parity = 0;

        return;
    }

    // Low-Nibble -> Offset des Bytes (relativ zu .minute),
    // zu dem das Bit gehört
    uint8_t offset = i & 0xf;
    uint8_t *d = &this->newtime.minute + offset;

    uint8_t val = *d;

    // Wertigkeit des Bits in BCD: 1, 2, 4, 8, 10, 20, 40, 80
    uint8_t bitval = this->bitval << 1;
    if (bitval == 0x10)
        bitval = 10;

    // Ein neuer Wert fängt an
    if (i & NEWVAL)
        val = 0, bitval = 1;

    this->bitval = bitval;

    // Falls DCF-Bit gesetzt ist: seine Wertigkeit zum
    // Zielbyte hinzuaddieren

    if (bit)
        val += bitval;

    *d = val;

    // Kleiner Konsistenztest: Testet Einer auf gültiges BCD-Format
    if (bitval == 8  &&  val >= 10)
        this->error = val;
}


