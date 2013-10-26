/**
 ******************************************************************************
 * @file        CLightHandler.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        15.10.2013
 * @project     ThunderCryer
 ******************************************************************************
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "CUSBVirtualCom.h"
#include "CRTCHandler.h"
#include "CWiFiHandler.h"
#include "cc3000.h"


extern chibios_rt::CUSBVirtualCom SDU2;
extern chibios_rt::CRTCHander rtcHandlerThread;
extern mbed_cc3000::CWiFiHandler wifiHandler;

extern void extcbAlarm(EXTDriver *extp, expchannel_t channel);
extern void extcbOneSec(EXTDriver *extp, expchannel_t channel);
extern void extcbWifi(EXTDriver *extp, expchannel_t channel);

#endif /* MAIN_H_ */
