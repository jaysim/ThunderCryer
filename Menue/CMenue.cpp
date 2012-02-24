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
	glcd->Inverse(true);
	glcd->Clear();


	vTaskDelay(500/portTICK_RATE_MS);
	glcd->WriteString("Introducing",c_FontSansSerif12,65,5);

	vTaskDelay(500/portTICK_RATE_MS);
	glcd->WriteString("Project:",c_FontSansSerif12,85,20);

	vTaskDelay(1000/portTICK_RATE_MS);
	glcd->WriteString("THUNDERCRYER",c_FontSansSerif12,0,35);

	vTaskDelay(3000/portTICK_RATE_MS);
	glcd->Window(0,0,117,50);
	glcd->WriteString("21:14",c_FontSansSerif26,10,5);
	glcd->WriteString("Fr. 24.02.12",c_FontSansSerif12,10,35);

	glcd->WriteString("Development in progress",c_FontNative,1,120);

	while(1){
		if(x<60) {
			sprintf(buffer,"%2d",x);
			glcd->WriteString(buffer,c_FontSansSerif12,80,7);
			x++;
			vTaskDelay(1000/portTICK_RATE_MS);
		} else
			x = 0;
	}
}
