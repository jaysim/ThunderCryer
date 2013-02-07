/**
  ******************************************************************************
  * @file      	CLedHeartBeat.hpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	14.02.12
  * @brief		Class for Led blinking on STM32F4Discovery Board
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CLEDHEARTBEAT_HPP_
#define CLEDHEARTBEAT_HPP_
/* Includes ------------------------------------------------------------------*/
#include "ManagedTask.h"

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
class CLedHeartBeat: public ManagedTask {

private:

	eLedState ledState;
	systime_t ledRate;

public:
	CLedHeartBeat(systime_t newRate);
	virtual ~CLedHeartBeat();
	void onCreate();
	msg_t main();
	void Rate(systime_t newRate);
	systime_t Rate() const;
	eLedState State() const;
};

#endif /* CLEDHEARTBEATSTM32F4DISC_H_ */
