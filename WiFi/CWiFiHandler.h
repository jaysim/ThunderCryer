/**
 ******************************************************************************
 * @file        CWifiHandler.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        14.10.2013
 * @project     ThunderCryer
 ******************************************************************************
 */

#ifndef CWIFIHANDLER_H_
#define CWIFIHANDLER_H_

#include "cc3000.h"
#include "NTPClient.h"
#include "main.h"
#include "ch.hpp"
#include "hal.h"


class CWiFiHandler : public chibios_rt::BaseStaticThread<2048>  {
private:
  NTPClient ntp;
  tUserFS user_info;
  cc3000 wifi;
  static const uint8_t smartconfigkey[];

  /**
   *  \brief First time configuration
   *  \param none
   *  \return none
   */
  void do_FTC(void);
protected:
  virtual msg_t main(void);

public:
  /**
   *  \brief Connect to SSID without security
   *  \param ssid Name of SSID
   *  \return none
   */
  void connect_to_ssid(char *ssid);

  /**
   *  \brief Connect to SSID with a timeout
   *  \param ssid     Name of SSID
   *  \param key      Password
   *  \param sec_mode Security mode
   *  \return none
   */
  void connect_to_ssid(char *ssid, char *key, unsigned char sec_mode);

  /**
   *  \brief Print cc3000 information
   *  \param none
   *  \return none
   */
  void print_cc3000_info();

  /*
   * callback for exti driver
   */
  void WLAN_IRQHandler();

  /**
   *  \brief Ctor.
   *  \param spidrv      Pointer to the spi driver resource
   *  \param ssport      The chip select line port.
   *  \param sspad       The chip select line pad number.
   *  \param cr1         SPI initialization data.
   *  \param irqport     The irq line port.
   *  \param irqpad      The irq line pad number.
   *  \param enport      The enable line port.
   *  \param enpad       The enable line pad number
   */
  CWiFiHandler(SPIDriver *spidrv, ioportid_t ssport, uint16_t sspad,
               uint16_t cr1, EXTDriver *extdrv,
               ioportid_t irqport, uint16_t irqpad,
               ioportid_t enport, uint16_t enpad);
  virtual ~CWiFiHandler();
};

#endif /* CWIFIHANDLER_H_ */
