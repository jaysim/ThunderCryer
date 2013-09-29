/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.hpp"
#include "hal.h"
#include "gfx.h"
#include "CDCF77.h"
#include "CRTCHandler.h"
#include "CRTCAlarm.h"
#include <ctime>


using namespace chibios_rt;

/*
 * LED blink sequences.
 * NOTE: Sequences must always be terminated by a GOTO instruction.
 * NOTE: The sequencer language could be easily improved but this is outside
 *       the scope of this demo.
 */
#define SLEEP           0
#define GOTO            1
#define STOP            2
#define BITCLEAR        3
#define BITSET          4

typedef struct {
  uint8_t       action;
  uint32_t      value;
} seqop_t;

// Flashing sequence for LED3.
static const seqop_t LED3_sequence[] =
{
  {BITSET, PAL_PORT_BIT(GPIOD_LED3)},
  {SLEEP,    800},
  {BITCLEAR,   PAL_PORT_BIT(GPIOD_LED3)},
  {SLEEP,    200},
  {GOTO,     0}
};

// Flashing sequence for LED4.
static const seqop_t LED4_sequence[] =
{
  {BITSET, PAL_PORT_BIT(GPIOD_LED4)},
  {SLEEP,    600},
  {BITCLEAR,   PAL_PORT_BIT(GPIOD_LED4)},
  {SLEEP,    400},
  {GOTO,     0}
};

// Flashing sequence for LED5.
static const seqop_t LED5_sequence[] =
{
  {BITSET, PAL_PORT_BIT(GPIOD_LED5)},
  {SLEEP,    400},
  {BITCLEAR,   PAL_PORT_BIT(GPIOD_LED5)},
  {SLEEP,    600},
  {GOTO,     0}
};

// Flashing sequence for LED6.
static const seqop_t LED6_sequence[] =
{
  {BITSET, PAL_PORT_BIT(GPIOD_LED6)},
  {SLEEP,    200},
  {BITCLEAR,   PAL_PORT_BIT(GPIOD_LED6)},
  {SLEEP,    800},
  {GOTO,     0}
};

/*
 * Sequencer thread class. It can drive LEDs or other output pins.
 * Any sequencer is just an instance of this class, all the details are
 * totally encapsulated and hidden to the application level.
 */
class SequencerThread : public BaseStaticThread<128> {
private:
  const seqop_t *base, *curr;                   // Thread local variables.

protected:
  virtual msg_t main(void) {

    setName("sequencer");

    while (true) {
      switch(curr->action) {
      case SLEEP:
        sleep(curr->value);
        break;
      case GOTO:
        curr = &base[curr->value];
        continue;
      case STOP:
        return 0;
      case BITCLEAR:
        palClearPort(GPIOD, curr->value);
        break;
      case BITSET:
        palSetPort(GPIOD, curr->value);
        break;
      }
      curr++;
    }
  }

public:
  SequencerThread(const seqop_t *sequence) : BaseStaticThread<128>() {

    base = curr = sequence;
  }
};


class ConsoleThread : public BaseStaticThread<2048> {
private:
  /* The handles for our three consoles */
  GHandle GW1;

protected:
  virtual msg_t main(void) {
    font_t  font1;
    Listener<CDCFNewTimeArrived,5> listenerDCF(&notifyDCFTime);
    Listener<CActualTime,10> listenerActTime(&notifyActTime);
    Listener<CActualTime,5> listenerActAlarm(&notifyActAlarm);

    setName("Console");

    /* Set some fonts */
    font1 = gdispOpenFont("Small");
    gwinSetDefaultFont(font1);

    /* create the three console windows */
    {
      GWindowInit     wi;

      wi.show = TRUE;
      wi.x = 0; wi.y = 0; wi.width = gdispGetWidth(); wi.height = gdispGetHeight();
      GW1 = gwinConsoleCreate(NULL, &wi);

    }

    /* Set the fore- and background colors for each console */
    gwinSetColor(GW1, Black);
    gwinSetBgColor(GW1, White);

    /* clear all console windows - to set background */
    gwinClear(GW1);

    /* Output some data on the first console */
    gwinPrintf(GW1, "Hello ChibiOS/GFX!\r\n");
    /*
     * Serves timer events.
     */
    while (true) {
      BaseThread::sleep(MS2ST(500));

      if(listenerDCF.available()){
        CDCFNewTimeArrived* dcf = listenerDCF.get();
        gwinPrintf(GW1, "DCF: ");
        gwinPrintf(GW1, ctime(&dcf->newTime));
        gwinPrintf(GW1, "\r\n");
        listenerDCF.release(dcf);
      }

      if(listenerActTime.available()){
        CActualTime* time = listenerActTime.get();
        gwinPrintf(GW1, "Time : ");
        gwinPrintf(GW1, ctime(&time->time));
        gwinPrintf(GW1, "\r\n");
        notifyActTime.release(time);
      }

      if(listenerActAlarm.available()){
        CActualTime* time = listenerActAlarm.get();
        gwinPrintf(GW1, "Alarm: ");
        gwinPrintf(GW1, ctime(&time->time));
        gwinPrintf(GW1, "\r\n");
        listenerActAlarm.release(time);
      }

    }
    return 0;
  }

public:
  ConsoleThread() {

  }
};



/* Static threads instances.*/
static SequencerThread blinker1(LED3_sequence);
static SequencerThread blinker2(LED4_sequence);
static SequencerThread blinker3(LED5_sequence);
static SequencerThread blinker4(LED6_sequence);
static CDCF77 dcfHandlerThread;
static CRTCHander rtcHandlerThread;
static ConsoleThread console;




/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();

  /* initialize and clear the display */
  gfxInit();


  /*
   * Starts several instances of the SequencerThread class, each one operating
   * on a different LED.
   */
  blinker1.start(NORMALPRIO + 10);
  blinker2.start(NORMALPRIO + 10);
  blinker3.start(NORMALPRIO + 10);
  blinker4.start(NORMALPRIO + 10);
  dcfHandlerThread.start(NORMALPRIO + 5);
  rtcHandlerThread.start(NORMALPRIO + 4);
  console.start(NORMALPRIO + 3);

  /*
   * Serves timer events.
   */
  while (true) {
    BaseThread::sleep(MS2ST(500));

  }

  return 0;
}
