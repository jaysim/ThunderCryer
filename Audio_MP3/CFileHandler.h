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

 extern "C" {
	#include "ff.h"
	#include "stdint.h"
	#include "mp3dec.h"
 }


/* Exported types ------------------------------------------------------------*/
typedef enum {
	PAUSE,
	OPEN_FILE,
	FIND_SYNC,
	DECODE,
	PLAYBACK,
	CLOSE_FILE
}ePlayMP3State;

typedef enum{
	BUFFER_1=0,
	BUFFER_2=1
}eBufferSelect;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */


/*
 *
 */
class CFileHandler: public AManagedTask{
private:


	FRESULT fsresult;               //return code for file related operations
	FIL file;                     	//file object
	DIR dir;						//directory object
	FILINFO fileInfo;

	ePlayMP3State eMP3State;
	eBufferSelect eBuffer;

	uint16_t uiBytesLeft;
	uint8_t * ptrReadPosition;

	uint8_t uiVolume;
	HMP3Decoder hMP3Decoder;
	MP3FrameInfo mp3FrameInfo;

	bool RefillBuffer(void);
	bool PlayMP3(const char * filename);

public:
	CFileHandler();
	virtual ~CFileHandler();
	bool HardwareInit();
	void Run();
};

#endif /* CFILEHANDLER_H_ */
