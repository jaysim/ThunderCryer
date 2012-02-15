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
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */


/**
  * @brief  Class for Led blinking on STM32F4Discovery board
  */
class CLedHeartBeatSTM32F4Disc:AManagedTask {
private:

public:
	CLedHeartBeatSTM32F4Disc();
	virtual ~CLedHeartBeatSTM32F4Disc();
};

#endif /* CLEDHEARTBEATSTM32F4DISC_H_ */
