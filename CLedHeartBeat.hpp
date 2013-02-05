/**
  ******************************************************************************
  * @file      	CLedHeartBeatSTM32F4Disc.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	14.02.12
  * @brief		Class for Led blinking on STM32F4Discovery Board
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CLEDHEARTBEATSTM32F4DISC_H_
#define CLEDHEARTBEATSTM32F4DISC_H_
/* Includes ------------------------------------------------------------------*/
#include "AManagedTask.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
	LED_Up,
	LED_Right,
	LED_Down,
	LED_Left
}eLedState;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */


/**
  * @brief  Class for Led blinking on STM32F4Discovery board
  */
class CLedHeartBeatSTM32F4Disc: public AManagedTask {

private:

	eLedState ledState;
	portTickType ledRate;

public:
	CLedHeartBeatSTM32F4Disc(portTickType newRate);
	virtual ~CLedHeartBeatSTM32F4Disc();
	bool HardwareInit();
	void Run();
	void Rate(portTickType newRate);
	portTickType Rate() const;
	eLedState State() const;
};

#endif /* CLEDHEARTBEATSTM32F4DISC_H_ */
