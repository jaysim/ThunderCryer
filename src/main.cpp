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
#include "CRTCAlarm.h"
#include "CWiFiHandler.h"
#include "main.h"


using namespace chibios_rt;
using namespace mbed_cc3000;

class ConsoleThread : public BaseStaticThread<2048> {
private:
  /* The handles for our three consoles */
  GHandle GW1;
  GHandle ghLabel1;
  GLabelObject gwLabel1;

protected:
  virtual msg_t main(void) {
    font_t  font1;
    Listener<CDCFNewTimeArrived,5> listenerDCF(&notifyDCFTime);
    Listener<CActualTime,10> listenerActTime(&notifyActTime);
    Listener<CActualTime,5> listenerActAlarm(&notifyActAlarm);

    setName("Console");

    /* Set some fonts */
    font1 = gdispOpenFont("DejaVuSans10");
    gwinSetDefaultFont(font1);

    gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);

    /* Set the fore- and background colors for each console */
    gwinSetColor(GW1, Black);
    gwinSetBgColor(GW1, White);

    /* create the three console windows */
    {
      GWindowInit     wi;
      GWidgetInit     wgi;

      // Apply some default values for GWIN
      wgi.customDraw = 0;
      wgi.customParam = 0;
      wgi.customStyle = 0;
      wgi.g.show = TRUE;

      // Apply the label parameters
      wgi.g.y = 0;
      wgi.g.x = 0;
      wgi.g.width = gdispGetWidth();
      wgi.g.height = 20;
      wgi.text = "Hello ChibiOS/GFX!";

      // Create the actual label
      ghLabel1 = gwinLabelCreate(&gwLabel1, &wgi);

      wi.show = TRUE;
      wi.x = 0; wi.y = 20; wi.width = gdispGetWidth(); wi.height = gdispGetHeight()-20;
      GW1 = gwinConsoleCreate(NULL, &wi);

    }


    /* clear all console windows - to set background */
    gwinClear(GW1);

    /* Output some data on the first console */
    gwinPrintf(GW1, "Hello ChibiOS/GFX!\n");
    /*
     * Serves timer events.
     */
    while (true) {
      BaseThread::sleep(MS2ST(500));

      if(listenerDCF.available()){
        CDCFNewTimeArrived* dcf = listenerDCF.get();
        gwinPrintf(GW1, "DCF: ");
        gwinPrintf(GW1, ctime(&dcf->newTime));
        USBCom.print("DCF: %s\n\r", ctime(&dcf->newTime));
        listenerDCF.release(dcf);
      }

      if(listenerActTime.available()){
        CActualTime* time = listenerActTime.get();
        gwinSetText(ghLabel1, ctime(&time->time), FALSE);
        notifyActTime.release(time);
      }

      if(listenerActAlarm.available()){
        CActualTime* time = listenerActAlarm.get();
        gwinPrintf(GW1, "Alarm: ");
        gwinPrintf(GW1, ctime(&time->time));
        USBCom.print("Alarm: %s\n\r", ctime(&time->time));
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
static CDCF77 dcfHandlerThread;
CRTCHander rtcHandlerThread;
static ConsoleThread console;
CWiFiHandler wifiHandler(&SPID1, GPIOC, GPIOC_WIFI_SS,
                                 SPI_CR1_BR_2 | SPI_CR1_CPHA,
                                 &EXTD1, GPIOC, GPIOC_WIFI_IRQ ,
                                 GPIOC, GPIOC_WIFI_EN);

CUSBVirtualCom USBCom;

/* Triggered when Wifi issues an event*/
void extcbWifi(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  wifiHandler.WLAN_IRQHandler();
}


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
  dcfHandlerThread.start(NORMALPRIO + 5);
  rtcHandlerThread.start(NORMALPRIO + 4);
  wifiHandler.start(NORMALPRIO + 3);
  console.start(NORMALPRIO + 2);
  USBCom.start(NORMALPRIO + 1);

  /*
   * Terminal Greetings
   */
  USBCom.print("ThunderCryer Debug Terminal\n\r");

  /*
   * Serves timer events.
   */
  while (true) {
    BaseThread::sleep(MS2ST(500));
  }

  return 0;
}
