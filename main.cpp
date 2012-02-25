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
#include "ustime.h"
#include "CLedHeartBeatSTM32F4Disc.h"
#include "CMenue.h"
#include "CT6963GPIOInterface.h"
#include "CFileHandler.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CLedHeartBeatSTM32F4Disc g_LedTast(500/portTICK_RATE_MS);
CT6963_GPIO_Interface g_Display;
CMenue	g_Menue(&g_Display);
CFileHandler g_FileHandler;

/* Private function prototypes -----------------------------------------------*/
extern "C" {
	void vApplicationTickHook( void );
	void vApplicationIdleHook( void );
	void vApplicationMallocFailedHook( void );
	void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName );
} // extern "C"
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  main function
  * @param  None
  * @retval None
  */
int main(void){
	unsigned int u;

	/**
	 * create Led Heartbeat task with minimal stack size and priority
	 * 1 higher than idle task
	 */
	//g_LedTast.Create("Led Heartbeat",configMINIMAL_STACK_SIZE,tskIDLE_PRIORITY+5);
	//g_Menue.Create("Menue",configMINIMAL_STACK_SIZE * 2,tskIDLE_PRIORITY+3);
	//g_FileHandler.Create("FileHandler",configMINIMAL_STACK_SIZE * 10,tskIDLE_PRIORITY+1);

	/**
	 * Initialise the hardware for all Tasks and
	 * then FreeRTOS takes control of execution
	 */
	//CFreeRTOS::InitHardwareForManagedTasks();
	//CFreeRTOS::StartScheduler();

	g_FileHandler.HardwareInit();

	g_FileHandler.Run();


	while(1){
		u++;
	}

	return 0;

}

extern "C" {

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

	// A required FreeRTOS function. gets called on stack overflow
	// ----------------------------------------------------------------------------
	void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName ){
		/* TODO, implement your code here */
		while(1);
	}

	/* User Callbacks: user has to implement these functions in his code if
	  they are needed. -----------------------------------------------------------*/

	uint16_t EVAL_AUDIO_GetSampleCallBack(void){
		return -1;
	}

	/* This function is called when the requested data has been completely transferred.
	   In Normal mode (when  the define AUDIO_MAL_MODE_NORMAL is enabled) this function
	   is called at the end of the whole audio file.
	   In circular mode (when  the define AUDIO_MAL_MODE_CIRCULAR is enabled) this
	   function is called at the end of the current buffer transmission. */
	void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){

	}

	/* This function is called when half of the requested buffer has been transferred
	   This callback is useful in Circular mode only (when AUDIO_MAL_MODE_CIRCULAR
	   define is enabled)*/
	void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size){

	}

	/* This function is called when an Interrupt due to transfer error on or peripheral
	   error occurs. */
	void EVAL_AUDIO_Error_CallBack(void* pData){

	}

	/* Codec_TIMEOUT_UserCallback() function is called whenever a timeout condition
	   occurs during communication (waiting on an event that doesn't occur, bus
	   errors, busy devices ...) on the Codec control interface (I2C).
	   You can use the default timeout callback implementation by uncommenting the
	   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_audio_codec.h file.
	   Typically the user implementation of this callback should reset I2C peripheral
	   and re-initialize communication or in worst case reset all the application. */
	uint32_t Codec_TIMEOUT_UserCallback(void){
		return -1;
	}

	/* LIS302DL_TIMEOUT_UserCallback() function is called whenever a timeout condition
	   occure during communication (waiting transmit data register empty flag(TXE)
	   or waiting receive data register is not empty flag (RXNE)).
	   You can use the default timeout callback implementation by uncommenting the
	   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_lis302dl.h file.
	   Typically the user implementation of this callback should reset MEMS peripheral
	   and re-initialize communication or in worst case reset all the application. */
	uint32_t LIS302DL_TIMEOUT_UserCallback(void){
		return -1;
	}
} // extern "C"


