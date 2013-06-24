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
#include "CTime.h"
#include "CT6963GPIOInterface.h"
#include "CUSBMassStorage.h"
#include "CFileHandler.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CLedHeartBeatSTM32F4Disc g_LedTast(200/portTICK_RATE_MS);
CT6963_GPIO_Interface g_Display;
CMenue	g_Menue(&g_Display);
CFileHandler g_FileHandler;
CUSB_MassStorage g_MSC;

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
	 * Unit Tests
	 */
	//Time::CTimeUnitTest();

	/**
	 * create Led Heartbeat task with minimal stack size and priority
	 * 1 higher than idle task
	 */
	g_LedTast.Create("Led Heartbeat",configMINIMAL_STACK_SIZE * 2, tskIDLE_PRIORITY+1);
	g_MSC.Create("USB Massstorage", configMINIMAL_STACK_SIZE * 10, tskIDLE_PRIORITY +2);
	g_Menue.Create("Menue",configMINIMAL_STACK_SIZE * 2, tskIDLE_PRIORITY+3);
	g_FileHandler.Create("FileHandler",configMINIMAL_STACK_SIZE * 10, tskIDLE_PRIORITY+10);

	/**
	 * Initialize the hardware for all Tasks and
	 * then FreeRTOS takes control of execution
	 */
	CFreeRTOS::InitHardwareForManagedTasks();
	CFreeRTOS::StartScheduler();

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
} // extern "C"

