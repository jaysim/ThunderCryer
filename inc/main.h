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



extern chibios_rt::CUSBVirtualCom SDU2;

extern void extcbAlarm(EXTDriver *extp, expchannel_t channel);
extern void extcbOneSec(EXTDriver *extp, expchannel_t channel);

#endif /* MAIN_H_ */
