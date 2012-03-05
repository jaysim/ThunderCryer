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
#include "stm32f4_sdio.h"
#include "misc.h"
#include "ff.h"
#include "string.h"
#include "coder.h"
#include "mp3common.h"
#include "mp3dec.h"
#include "stm32f4_discovery_audio_codec.h"
/* Private typedef -----------------------------------------------------------*/
/*******************************
 * These are possible application
 * states.
 ******************************/
typedef enum
{
    IDLE_STATE,
    DEVICE_ATTACHED,
    SEARCH_MP3_FILES,
    OPEN_MP3_FILE,
    FILE_OPEN,
    FRAME_SYNC_FIND,
    FILE_READ,
    MP3_DECODE,
    PLAYBACK,
    FILE_CLOSE,
    PLAYBACK_PAUSE,
    PLAYBACK_STOP,
    SYSTEM_HALT

}APPLICATION_STATE;
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE		4096
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
APPLICATION_STATE applicationState,previousAppState;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  filles readbuffer with new data from file
  * @param  readBuf  pointer to data storage for read data
  * @param  readPtr  actual read position
  * @retval None
  */
static int FillReadBuffer(unsigned char *readBuf, unsigned char *readPtr, int bufSize, int bytesLeft, FIL *infile)
{
    /* This function will move bytesLeft bytes from
     * readPtr to the top of readBuf. bufSize is the
     * size of readBuf. It then reads bufSize - bytesLeft bytes
     * from infile and appends these bytes to readBuf.
     * If readBuf is not full, then remaining bytes are
     * set to zero. The total number of bytes read from
     * infile is returned.
     */

    unsigned int uiBytesRead;
    FRESULT fsresult=FR_OK;

    /* move last, small chunk from end of buffer to start, then fill with new data */
    memmove(readBuf, readPtr, bytesLeft);
	fsresult = f_read(infile,readBuf + bytesLeft,bufSize - bytesLeft, &uiBytesRead);
	if (fsresult != FR_OK){
		//File read failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning
		while(1);
	}
    /* zero-pad to avoid finding false sync word after last frame (from old data in readBuf) */
    if (uiBytesRead < (bufSize - bytesLeft))
        memset(readBuf + bytesLeft + uiBytesRead, 0, bufSize - bytesLeft - uiBytesRead);

    return uiBytesRead;
}


CFileHandler::CFileHandler() {
	Status = SD_OK;
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

	uint8_t uiReadBuffer[BUFFER_SIZE];
	unsigned int uiBytesRead=0;
	HMP3Decoder mp3Decoder;

	/*-----------------------------------------------------------------------------
	Initialize the MP3decoder structure
	-----------------------------------------------------------------------------*/
	mp3Decoder = MP3InitDecoder();
	if(mp3Decoder == 0) {
		// memory allocation failed, 28k heap required
		while(1);
	}

	/*-----------------------------------------------------------------------------
	Initialize the audio dac and codec
	-----------------------------------------------------------------------------*/
	EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO,100,44100);

	/*-----------------------------------------------------------------------------
	Beginning of FAT file system related code.  The following code shows steps
	necessary to create, read, and write files.
	See http://elm-chan.org/fsw/ff/00index_e.html for more information.
	This code assumes a single logical drive, drive number 0.  It also assumes a single partition.
	-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
	Mount the file system on logical drive 0.  Mounting associates the file system
	structure with a logical drive number.  Mounting must be done before any file
	operations.  Mounting does not write anything to the SD card, it simply
	initializes and associates the file structure.  The file system structure
	members are not filled in until the first file operation after f_mount.
	-----------------------------------------------------------------------------*/
	fsresult = f_mount(0, &myfs);

	if (fsresult != FR_OK){
		//FAT file system mounting failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.
		while(1);
	}

	/*-----------------------------------------------------------------------------
	Open a file and read first buffer
	-----------------------------------------------------------------------------*/
	fsresult = f_open(&myfile, "A-Team.mp3", FA_OPEN_EXISTING | FA_READ);  //open file for read
	if (fsresult != FR_OK){
		//File open failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning
		while(1);
	}
	f_sync(&myfile);   // minimize critical section, see Appnote from fatfs

	/*-----------------------------------------------------------------------------
	read buffer out of file
	-----------------------------------------------------------------------------*/
	fsresult = f_read(&myfile,uiReadBuffer,BUFFER_SIZE,&uiBytesRead);
	if (fsresult != FR_OK){
		//File read failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning
		while(1);
	}
	/*-----------------------------------------------------------------------------
	Decode Mp3 file, to end of file
	-----------------------------------------------------------------------------*/






	/* Infinite loop */
	while (1)
	{}
}
}








