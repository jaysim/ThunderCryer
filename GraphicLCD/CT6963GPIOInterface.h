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
private:

	/**
	  * @brief  initialize Hardware to drive the display
	  * @param  None
	  * @retval None
	  */
	void HardwareInit(void);

	/**
	  * @brief  Reset the Display
	  * @param  None
	  * @retval None
	  */
	void Reset(void);

	/**
	  * @brief  writes a command
	  * @param  command
	  * @retval None
	  */
	void WriteCommand(const unsigned char command);

	/**
	  * @brief  writes a data
	  * @param  data
	  * @retval None
	  */
	void WriteData(const unsigned char data);

	/**
	  * @brief  reads data from display
	  * @param  None
	  * @retval data from display
	  */
	unsigned char ReadData() const;

public:
	CT6963_GPIO_Interface();
	virtual ~CT6963_GPIO_Interface();
};

#endif /* CT6963GPIOINTERFACE_H_ */
