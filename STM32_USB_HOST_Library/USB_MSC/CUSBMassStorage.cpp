/**
  ******************************************************************************
  * @file      	UUSBMassStorage.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	10.03.12
  * @brief		Class for USB MSC handling task
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "CUSBMassStorage.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "usbh_usr.h"
#include "usb_hcd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_OTG_CORE_HANDLE          USB_OTG_Core;
USBH_HOST                    USB_Host;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



CUSB_MassStorage::CUSB_MassStorage() {
	// TODO Auto-generated constructor stub

}

CUSB_MassStorage::~CUSB_MassStorage() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * 		USB OTG FS will be initialized interface will be initialized
  * @param  None
  * @retval true on succsess
  */
bool CUSB_MassStorage::HardwareInit(void){

	/* Init Host Library */
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);

	return true;
}

/**
  * @brief  task function for USB handling
  * @param  None
  * @retval None
  */
void CUSB_MassStorage::Run(void){
	while(1){
	    /* Host Task handler */
	    USBH_Process(&USB_OTG_Core, &USB_Host);
            CTask::Delay(25 / portTICK_RATE_MS);
	}
}

/**
  * @brief  Get Connection Status
  * @param  None
  * @retval status of storage device
  */
bool CUSB_MassStorage::IsDeviceConnected(void){
	return HCD_IsDeviceConnected(&USB_OTG_Core);
}



