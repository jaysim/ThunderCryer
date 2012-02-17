/**
  ******************************************************************************
  * @file      	CLedHeartBeatSTM32F4Disc.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	17.02.12
  * @brief		Class T6963C GPIO interface
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CT6963GPIOINTERFACE_H_
#define CT6963GPIOINTERFACE_H_
/* Includes ------------------------------------------------------------------*/
#include "CT6963CDriver.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */


class CT6963_GPIO_Interface: public CT6963CDriver {
public:
	CT6963_GPIO_Interface();
	virtual ~CT6963_GPIO_Interface();
};

#endif /* CT6963GPIOINTERFACE_H_ */
