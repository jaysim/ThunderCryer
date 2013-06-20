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
#include "CT6963CDriver.h"
#include "SansSerif12.h"
#include "SansSerif26.h"
#include <cstdio>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CMenue::CMenue(CGraphicLCD *display) : glcd(display){
}

CMenue::~CMenue() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * @param  None
  * @retval true on success
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
	char buffer[4];
	int x = 0;

	glcd->Init();
	glcd->Inverse(false);
	glcd->Clear();


	vTaskDelay(500/portTICK_RATE_MS);
	glcd->WriteString("Introducing Project:",c_FontSansSerif12,5,5);

	vTaskDelay(1000/portTICK_RATE_MS);
	glcd->WriteString("ThunderCryer",c_FontSansSerif26,3,35);

	vTaskDelay(10000/portTICK_RATE_MS);
	glcd->Clear();
	glcd->Window(2,2,100,60);
	glcd->WriteString("21:14",c_FontSansSerif26,5,5);
	glcd->WriteString("Fr. 24.02.12",c_FontNative,5,40);

	glcd->WriteString("Development in progress",c_FontNative,1,120);

	while(1){
		if(x<60) {
			sprintf(buffer,"%02u",x);
			glcd->SectorClear(75,7,20,20);
			glcd->WriteString(buffer,c_FontSansSerif12,75,7);
			x++;
			vTaskDelay(1000/portTICK_RATE_MS);
		} else
			x = 0;
	}
}
