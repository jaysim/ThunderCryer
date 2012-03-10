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
#include "CUSBMassStorage.h"
#include "stm32f4xx.h"
#include "misc.h"
#include "string.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define READBUF_SIZE	8192

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CUSB_MassStorage g_MSC;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CFileHandler::CFileHandler() {
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
	UINT bytesWritten;

	//wait for USB Stick getting mounted
	while(!g_MSC.DeviceMounted());

	fsresult = f_open(&myfile,"0:Hello.txt",FA_CREATE_ALWAYS | FA_WRITE);
	fsresult = f_write(&myfile,"Hello USB Stick!",17,&bytesWritten);
	fsresult = f_close(&myfile);


	while(1){

	}
}
}










