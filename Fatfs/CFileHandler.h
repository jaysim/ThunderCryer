/**
  ******************************************************************************
  * @file      	CFileHandler.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	23.02.12
  * @brief		Class the file handling task
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CFILEHANDLER_H_
#define CFILEHANDLER_H_
/* Includes ------------------------------------------------------------------*/
#include "AManagedTask.h"
#include "stm32f4_sdio.h"
#include "ff.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */


/*
 *
 */
class CFileHandler: public AManagedTask{
private:
	SD_Error Status;
	SD_CardInfo SDCardInfo;

	FRESULT fsresult;               //return code for file related operations
	FATFS myfs;                     //FAT file system structure, see ff.h for details
	FIL myfile;                     //file object
public:
	CFileHandler();
	virtual ~CFileHandler();
	bool HardwareInit();
	void Run();
};

#endif /* CFILEHANDLER_H_ */