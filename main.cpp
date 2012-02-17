/**
  ******************************************************************************
  * @file      	main.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	14.02.12
  * @brief		main program file for Project: ThunderCryer
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CFreeRTOS.h"
#include "CTask.h"
#include "CLedHeartBeatSTM32F4Disc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CLedHeartBeatSTM32F4Disc g_LedTast(500/portTICK_RATE_MS);
/* Private function prototypes -----------------------------------------------*/
void vApplicationTickHook( void );
void vApplicationIdleHook( void );
void vApplicationMallocFailedHook( void );
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  main function
  * @param  None
  * @retval None
  */
int main(void){

	/**
	 * create Led Heartbeat task with minimal stack size and priority
	 * 1 higher than idle task
	 */
	g_LedTast.Create("Led Heartbeat",configMINIMAL_STACK_SIZE,tskIDLE_PRIORITY+1);

	/**
	 * Initialise the hardware for all Tasks and
	 * then FreeRTOS takes control of execution
	 */
	CFreeRTOS::InitHardwareForManagedTasks();
	CFreeRTOS::StartScheduler();

}

// This FreeRTOS callback function gets called once per tick (default = 1000Hz).
// ----------------------------------------------------------------------------
void vApplicationTickHook( void ) {
	// for display delays needs 65ms call rate to prevent overflow
	get_us_time();
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


/*
  * Callback used by stm32f4_discovery_audio_codec.c.
  * Refer to stm32f4_discovery_audio_codec.h for more info.
  */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
   /* TODO, implement your code here */
   return;
}

/*
  * Callback used by stm324xg_eval_audio_codec.c.
  * Refer to stm324xg_eval_audio_codec.h for more info.
  */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
   /* TODO, implement your code here */
   return -1;
}



