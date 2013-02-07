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
#include "ch.hpp"
#include "hal.h"
#include "CLedHeartBeat.hpp"
/* namespaces ----------------------------------------------------------------*/
using namespace chibios_rt;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static CLedHeartBeat heartBeatThread(NORMALPRIO,"HeartBeat");
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  main function
  * @param  None
  * @retval None
  */
int main(void){
	unsigned int u;

	/*
	 * init every thing
	 */
	halInit();
	System::init();
	ManagedTask::startAll();


	while(1){
		u++;
	}

	return 0;

}
