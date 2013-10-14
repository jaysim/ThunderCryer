/**
 ******************************************************************************
 * @file        CLightHandler.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        14.10.2013
 * @project     ThunderCryer
 ******************************************************************************
 */

#ifndef CUSBVIRTUALCOM_H_
#define CUSBVIRTUALCOM_H_

#include <stdint.h>
#include "ch.hpp"
#include "hal.h"


namespace chibios_rt {

  class CUSBVirtualCom : public BaseStaticThread<2048>{
  private:

    /*===========================================================================*/
    /* USB related stuff.                                                        */
    /*===========================================================================*/

    /*
     * Endpoints to be used for USBD2.
     */
    static const uint8_t USBD2_DATA_REQUEST_EP;
    static const uint8_t USBD2_DATA_AVAILABLE_EP;
    static const uint8_t USBD2_INTERRUPT_REQUEST_EP;

    /*
     * Serial over USB Driver structure.
     */
    static SerialUSBDriver SDU2;

    /*
     * USB Device Descriptor.
     */
    static const uint8_t vcom_device_descriptor_data[18];

    /*
     * Device Descriptor wrapper.
     */
    static const USBDescriptor vcom_device_descriptor;

    /* Configuration Descriptor tree for a CDC.*/
    static const uint8_t vcom_configuration_descriptor_data[67];

    /*
     * Configuration Descriptor wrapper.
     */
    static const USBDescriptor vcom_configuration_descriptor;

    /*
     * U.S. English language identifier.
     */
    static const uint8_t vcom_string0[];

    /*
     * Vendor string.
     */
    static const uint8_t vcom_string1[];

    /*
     * Device Description string.
     */
    static const uint8_t vcom_string2[];

    /*
     * Serial Number string.
     */
    // TODO: Version anpassen
    static const uint8_t vcom_string3[];

    /*
     * Strings wrappers array.
     */
    static const USBDescriptor vcom_strings[];

    /**
     * @brief   IN EP1 state.
     */
    static USBInEndpointState ep1instate;

    /**
     * @brief   OUT EP1 state.
     */
    static USBOutEndpointState ep1outstate;

    /**
     * @brief   EP1 initialization structure (both IN and OUT).
     */
    static const USBEndpointConfig ep1config;

    /**
     * @brief   IN EP2 state.
     */
    static USBInEndpointState ep2instate;

    /**
     * @brief   EP2 initialization structure (IN only).
     */
    static const USBEndpointConfig ep2config;

    /*
     * USB driver configuration.
     */
    static const USBConfig usbcfg;

    /*
     * Serial over USB driver configuration.
     */
    static const SerialUSBConfig serusbcfg;


    /*
     * Handles the GET_DESCRIPTOR callback. All required descriptors must be
     * handled here.
     */
    static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                               uint8_t dtype,
                                               uint8_t dindex,
                                               uint16_t lang);

    /*
     * Handles the USB driver global events.
     */
    static void usb_event(USBDriver *usbp, usbevent_t event);
  protected:
    virtual msg_t main(void);

  public:
    CUSBVirtualCom();
    virtual ~CUSBVirtualCom();
  };

} /* namespace chibios_rt */
#endif /* CUSBVIRTUALCOM_H_ */
