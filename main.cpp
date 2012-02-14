/**
  ******************************************************************************
  * @file      	main.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	14.02.12
  * @brief		main program file for Project: ThunderCryer
  */

#include "FreeRTOS.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	LED_Up,
	LED_Right,
	LED_Down,
	LED_Left
}eLedState;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void vHardwareInit(void);
void vLedTask(void* pvParameter);
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  main function
  * @param  None
  * @retval None
  */
int main(void){
	vHardwareInit();

}

// This FreeRTOS callback function gets called once per tick (default = 1000Hz).
// ----------------------------------------------------------------------------
void vApplicationTickHook( void ) {

}

// This FreeRTOS call-back function gets when no other task is ready to execute.
// On a completely unloaded system this is getting called at over 2.5MHz!
// ----------------------------------------------------------------------------
void vApplicationIdleHook( void ) {

}

// A required FreeRTOS function.
// ----------------------------------------------------------------------------
void vApplicationMallocFailedHook( void ) {
    configASSERT( 0 );  // Latch on any failure / error.
}

/**
  * @brief  hardware initialisation
  * @param  None
  * @retval None
  */
void vHardwareInit(void){

}

/**
  * @brief  led blinking task
  * @param  task parameter
  * @retval None
  */
void vLedTask(void* pvParameter){
	eLedState ledState = LED_Up;

	// GPIO init
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);


	while(1){
		switch(ledState){
		case LED_Up:
			STM_EVAL_LEDOn(LED3);
			STM_EVAL_LEDOff(LED4);
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOff(LED6);
			ledState = LED_Right;
			break;
		case LED_Right:
			STM_EVAL_LEDOn(LED5);
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED4);
			STM_EVAL_LEDOff(LED6);
			ledState = LED_Down;
			break;
		case LED_Down:
			STM_EVAL_LEDOn(LED6);
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED4);
			STM_EVAL_LEDOff(LED5);
			ledState = LED_Left;
			break;
		case LED_Left:
			STM_EVAL_LEDOn(LED4);
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOff(LED6);
			ledState = LED_Up;
			break;
		default:
			ledState = LED_Up;
			break;
		}

	}
}


