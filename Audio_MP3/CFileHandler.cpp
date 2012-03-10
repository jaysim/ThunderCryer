/**
  ******************************************************************************
  * @file      	CFileHandler.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	23.02.12
  * @brief		Class the file handling task
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CFileHandler.h"
#include "stm32f4xx.h"
#include "misc.h"
#include "string.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define READBUF_SIZE	8192

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef MEDIA_USB_KEY
 USB_OTG_CORE_HANDLE          USB_OTG_Core;
 USBH_HOST                    USB_Host;
#endif
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CFileHandler::CFileHandler() {
#ifdef STM32_SDIO
	Status = SD_OK;
#endif
	fsresult = FR_OK;

}

CFileHandler::~CFileHandler() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * 		SD Card interface will be initialized
  * @param  None
  * @retval true on succsess
  */
bool CFileHandler::HardwareInit(){
	return true;
}

/**
  * @brief  task function for Led Heartbeat
  * @param  None
  * @retval None
  */
void CFileHandler::Run(){

	unsigned int bytesLeft;
	int nRead, offset, eofReached, err;

	/* Init Host Library */
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);




	while(1){
	    /* Host Task handler */
	    USBH_Process(&USB_OTG_Core, &USB_Host);
	}


}
}









