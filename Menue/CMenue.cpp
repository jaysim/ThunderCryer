/**
  ******************************************************************************
  * @file      	CMenue.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	18.02.12
  * @brief		Class for Menu displayed on GraphicsLCD
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CMenue.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CMenue::CMenue(CGraphicLCD display) : glcd(display){
}

CMenue::~CMenue() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * @param  None
  * @retval true on succsess
  */
bool CMenue::HardwareInit(void){

	return true;
}


/**
  * @brief  task function for Menu
  * @param  None
  * @retval None
  */
void CMenue::Run(void){
	glcd.Init();
	glcd.Inverse(true);
	glcd.Clear();


	glcd.Window(0,0,119,63);
	glcd.Window(121,0,119,63);
	glcd.Window(0,65,119,63);
	glcd.Window(121,65,119,63);


	while(1){



	}
}
