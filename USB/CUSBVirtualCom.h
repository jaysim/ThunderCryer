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

namespace chibios_rt {
  /*
   * Serial over USB Driver structure.
   */
  extern static SerialUSBDriver SDU2;

  class CUSBVirtualCom {
  public:
    CUSBVirtualCom();
    virtual ~CUSBVirtualCom();
  };

} /* namespace chibios_rt */
#endif /* CUSBVIRTUALCOM_H_ */
