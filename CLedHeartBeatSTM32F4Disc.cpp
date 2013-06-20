/**
  ******************************************************************************
  * @file      	CLedHeartBeatSTM32F4Disc.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	14.02.12
  * @brief		Class for Led blinking on STM32F4Discovery Board
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CLedHeartBeatSTM32F4Disc.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CLedHeartBeatSTM32F4Disc::CLedHeartBeatSTM32F4Disc(portTickType newRate) {
	ledState = LED_Up;
	ledRate = newRate;
}

CLedHeartBeatSTM32F4Disc::~CLedHeartBeatSTM32F4Disc() {
	// nothing to destroy
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * @param  None
  * @retval true on succsess
  */
bool CLedHeartBeatSTM32F4Disc::HardwareInit(){

	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	return true;
}

/**
  * @brief  task function for Led Heartbeat
  * @param  None
  * @retval None
  */
void CLedHeartBeatSTM32F4Disc::Run(){
	while(1){
		switch(ledState){
		case LED_Up:
			STM_EVAL_LEDOn(LED3);
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOff(LED6);
			STM_EVAL_LEDOff(LED4);
			ledState = LED_Right;
			break;
		case LED_Right:
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOn(LED5);
			STM_EVAL_LEDOff(LED6);
			STM_EVAL_LEDOff(LED4);
			ledState = LED_Down;
			break;
		case LED_Down:
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOn(LED6);
			STM_EVAL_LEDOff(LED4);
			ledState = LED_Left;
			break;
		case LED_Left:
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOff(LED6);
			STM_EVAL_LEDOn(LED4);
			ledState = LED_Up;
			break;
		default:
			ledState = LED_Up;
			break;
		}
		CTask::Delay(ledRate); // 500ms delay
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
