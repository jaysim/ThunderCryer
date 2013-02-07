/**
  ******************************************************************************
  * @file      	CLedHeartBeat.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	14.02.12
  * @brief		Class for Led blinking on STM32F4Discovery Board
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CLedHeartBeat.hpp"
#include "hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CLedHeartBeat::CLedHeartBeat(systime_t newRate) {
  ledState = LED_Up;
  ledRate = newRate;
}

CLedHeartBeat::~CLedHeartBeat() {
  // nothing to destroy
}


/**
 * @brief  task function for Led Heartbeat
 * @param  None
 * @retval None
 */
msg_t CLedHeartBeat::main(){
  while(1){
    switch(ledState){
    case LED_Up:
      palWriteGroup(GPIOD,0x0F,GPIOD_LED4,1);
      ledState = LED_Right;
      break;
    case LED_Right:
      palWriteGroup(GPIOD,0x0F,GPIOD_LED4,4);
      ledState = LED_Down;
      break;
    case LED_Down:
      palWriteGroup(GPIOD,0x0F,GPIOD_LED4,8);
      ledState = LED_Left;
      break;
    case LED_Left:
      palWriteGroup(GPIOD,0x0F,GPIOD_LED4,2);
      ledState = LED_Up;
      break;
    default:
      ledState = LED_Up;
      break;
    }
    sleep(ledRate); // delay
  }
}

/**
  * @brief  set led blinking rate
  * @param  new rate in ticks
  * @retval None
  */
inline void CLedHeartBeatSTM32F4Disc::Rate(portTickType newRate){
	ledRate = newRate;
}

/**
  * @brief  get led blinking rate
  * @param  None
  * @retval actual blink rate
  */
inline portTickType CLedHeartBeatSTM32F4Disc::Rate() const{
	return ledRate;
}

/**
  * @brief  get the actual blink state (led position)
  * @param  None
  * @retval blink state enum
  */
inline eLedState CLedHeartBeatSTM32F4Disc::State() const{
	return ledState;
}
