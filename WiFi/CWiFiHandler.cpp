/**
 ******************************************************************************
 * @file        CWifiHandler.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        14.10.2013
 * @project     ThunderCryer
 ******************************************************************************
 */

#include "CWiFiHandler.h"
#include "stdio.h"

using namespace mbed_cc3000;



#ifndef CC3000_UNENCRYPTED_SMART_CONFIG
  const uint8_t CWiFiHandler::smartconfigkey[] = {0x73,0x6d,0x61,0x72,0x74,0x63,0x6f,0x6e,0x66,0x69,0x67,0x41,0x45,0x53,0x31,0x36};
#else
  const uint8_t CWiFiHandler::smartconfigkey[] = {0};
#endif

  CWiFiHandler::CWiFiHandler(SPIDriver *spidrv, ioportid_t ssport, uint16_t sspad,
                             uint16_t cr1, EXTDriver *extdrv,
                             ioportid_t irqport, uint16_t irqpad,
                             ioportid_t enport, uint16_t enpad):
                             wifi(spidrv, ssport, sspad, cr1, extdrv, irqport,
                                  irqpad, enport, enpad)
  {


  }
  void CWiFiHandler::WLAN_IRQHandler(){
    wifi._spi.WLAN_IRQHandler();
  }

  CWiFiHandler::~CWiFiHandler() {
    // TODO Auto-generated destructor stub
  }

  /**
   *  \brief NTP client demo
   *  \param  none
   *  \return int
   */
  msg_t CWiFiHandler::main(void) {

    setName("WiFiHandler");

    wifi.start(0);

    sleep(MS2ST(100));

    while(1){

      printf("cc3000 NTP client. \r\n");
      print_cc3000_info();

      printf("Attempting SSID Connection. \r\n");

      if (user_info.FTC == 1) {
        wifi._wlan.ioctl_set_connection_policy(0, 1, 1);
      } else {
        printf("Smart config is not set, starting configuration. \r\n");
        do_FTC();
        printf("Smart config is set. Please restart your board. \r\n");
        while(1);
      }

      printf("DHCP request \r\n");
      while (wifi.is_dhcp_configured() == false) {
        sleep(MS2ST(500));
        printf("  Waiting for dhcp to be set. \r\n");
      }


      while(wifi.is_connected()){

        printf("Getting time, 10s timeout. \r\n");
        if (ntp.setTime("0.uk.pool.ntp.org") == 0)
        {
          printf("Set time successfully. \r\n");
          time_t ctTime;
          ctTime = time(NULL);
          printf("Time is set to (UTC): %s \r\n", ctime(&ctTime));
        }
        else
        {
          printf("Error. \r\n");
        }

        sleep(MS2ST(100));
      }
      wifi.disconnect();
    }
    return 0;
  }

  /**
   *  \brief First time configuration
   *  \param none
   *  \return none
   */
  void CWiFiHandler::do_FTC(void) {
    printf("Running First Time Configuration \r\n");
    wifi.start_smart_config(smartconfigkey);
    while (wifi.is_dhcp_configured() == false) {
      sleep(MS2ST(500));
      printf("Waiting for dhcp to be set. \r\n");
    }
    user_info.FTC = 1;
    wifi.set_user_file_info((uint8_t *)&user_info, sizeof(user_info));
    wifi._wlan.stop();
    printf("FTC finished. \r\n");
  }

  /**
   *  \brief Connect to SSID without security
   *  \param ssid Name of SSID
   *  \return none
   */
  void CWiFiHandler::connect_to_ssid(char *ssid) {
    wifi.connect_open((uint8_t *)ssid);
  }

  /**
   *  \brief Connect to SSID with a timeout
   *  \param ssid     Name of SSID
   *  \param key      Password
   *  \param sec_mode Security mode
   *  \return none
   */
  void CWiFiHandler::connect_to_ssid(char *ssid, char *key, unsigned char sec_mode) {
    printf("Connecting to SSID: %s. Timeout is 10s. \r\n",ssid);
    if (wifi.connect_to_AP((uint8_t *)ssid, (uint8_t *)key, sec_mode) == true) {
      printf(" Connected. \r\n");
    } else {
      printf(" Connection timed-out (error). Please restart. \r\n");
      while(1);
    }
  }

  /**
   *  \brief Print cc3000 information
   *  \param none
   *  \return none
   */
  void CWiFiHandler::print_cc3000_info() {
    uint8_t myMAC[8];

    printf("MAC address + cc3000 info \r\n");
    wifi.get_user_file_info((uint8_t *)&user_info, sizeof(user_info));
    wifi.get_mac_address(myMAC);
    printf(" MAC address %02x:%02x:%02x:%02x:%02x:%02x \r\n \r\n", myMAC[0], myMAC[1], myMAC[2], myMAC[3], myMAC[4], myMAC[5]);

    printf(" FTC        %i \r\n",user_info.FTC);
    printf(" PP_version %i.%i \r\n",user_info.PP_version[0], user_info.PP_version[1]);
    printf(" SERV_PACK  %i.%i \r\n",user_info.SERV_PACK[0], user_info.SERV_PACK[1]);
    printf(" DRV_VER    %i.%i.%i \r\n",user_info.DRV_VER[0], user_info.DRV_VER[1], user_info.DRV_VER[2]);
    printf(" FW_VER     %i.%i.%i \r\n",user_info.FW_VER[0], user_info.FW_VER[1], user_info.FW_VER[2]);
  }


