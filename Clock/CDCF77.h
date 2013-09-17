/**
 ******************************************************************************
 * @file        CDCF77.h
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

#ifndef CDCF77_H_
#define CDCF77_H_

#include "ch.hpp"
#include <ctime>
#include <stdint.h>
#include "Notifier.h"

using namespace std;

namespace chibios_rt {


typedef struct
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t day_of_week;
  uint8_t month;
  uint8_t year;
#ifdef TIME_MESZ
  uint8_t mesz;
#endif // TIME_MESZ
} dcftime_t;


/*
 *
 */
class CDCF77 : public BaseStaticThread<128>{
private:
  /* Codiert die DCF-Bits 17..58 bzw. 21...58 (je nachdem, ob MESZ
 empfangen werden soll) und deren Position in time_t.
 Low-Nibble     : Offset relativ zu .minute
 Bit 4 (NEWVAL) : Neuer Wert fängt an, d.h. die Speicherposition
                  muss 1 Byte voranschreiten. */
  static const uint8_t dcf_byteno[];

  // In newtime wird die neue Zeit-Information aufgebaut
  static dcftime_t newtime;

  // Bei korrekt empfangener Zeit wird die Zeit nach *ptime kopiert.
  //
  // !!! *ptime MUSS AUSSERHALB DES DCF77-MODULS GESETZT WERDEN  !!!
  // !!! ANSONSTEN WIRD DIE ZEIT NICHT HINEINKOPIERT             !!!
  dcftime_t ptime;


  // Wenn die neue Zeit nach *ptime kopiert wird, wird time_changed
  // auf 1 gesetzt. Die Anwendung kann time_changed wieder auf
  // 0 zurücksetzten. So kann die Anwendung erkennen, daß ein neues
  // Zeit/Datums-Paket angekommen ist.
  uint8_t time_changed;

  // Für interne Verwendung in dcf77.c

  // Parity wird on the fly mitberechnet
  uint8_t parity;

  // Es gab einen Fehler: error != 0
  uint8_t error;

  // Wertigkeit des aktuellen BCD-Bits: 1, 2, 4, 8, 10, 20, 40, 80.
  uint8_t bitval;

  // Dauer der aktuellen Sekunde in Einheiten von 10ms (1 Tick)
  uint8_t ticks;

  // Letzter poll-Wert vom Aufruf von dcf77_tick() zur
  // Flankenerkennung.
  uint8_t poll;

  uint8_t byteno;

  void StartBit(uint8_t ticks);
  void StoreBit(uint8_t ticks);
  void EndBit(uint8_t bit);
protected:

  virtual msg_t main(void);

};


/**
 * @class CDCFNewTimeArrived
 *
 * @brief signal for incomming new dcf time
 *
 */
class CDCFNewTimeArrived : public NotifierMsg<CDCFNewTimeArrived> {
public:
    time_t newTime;
};


extern Notifier<CDCFNewTimeArrived> notifyDCFTime;



} /* namespace chibios_rt */
#endif /* CDCF77_H_ */

