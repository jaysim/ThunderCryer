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

/* Private define ------------------------------------------------------------*/
#define READBUF_SIZE	1940
BYTE    readBuf [READBUF_SIZE];
BYTE   *readPtr;
int   outBuf  [READBUF_SIZE];
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
	applicationState = IDLE_STATE;
	previousAppState = IDLE_STATE;
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

	HMP3Decoder mp3Decoder;
	MP3FrameInfo mp3FrameInfo;
	unsigned int bytesLeft;
	int nRead, offset, eofReached, err;

	/*-----------------------------------------------------------------------------
	Initialize the MP3decoder structure
	-----------------------------------------------------------------------------*/
	mp3Decoder = MP3InitDecoder();
	if(mp3Decoder == 0) {
		// memory allocation failed, 28k heap required
		while(1);
	}

	EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_I2S);

	while(1){
	/*-----------------------------------------------------------------------------
	Statemachine
	-----------------------------------------------------------------------------*/
		switch(applicationState)
		{
		case(IDLE_STATE):
			/* not implemented yet */
			applicationState = OPEN_MP3_FILE;

			break;

/*------------------------------------------------------------------------------*/
		case(OPEN_MP3_FILE):
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
			Open a file
			-----------------------------------------------------------------------------*/
			fsresult = f_open(&myfile, "A-Team.mp3", FA_OPEN_EXISTING | FA_READ);  //open file for read
			if (fsresult != FR_OK){
				//File open failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning
				while(1);
			}
			f_sync(&myfile);   // minimize critical section, see Appnote from fatfs

			applicationState = FILE_OPEN ;  //File is open change state

			break;

/*------------------------------------------------------------------------------*/
		case(FILE_OPEN):

			/* Intialize the MP3 decoder variables
			 * as we will start decoding a new file.
			 * The details of the MP3 file are obtained
			 * and the read pointer is set up to point
			 * to the start of the MP3 frame.*/

			/* At this point, the MP3 decoder is already
			 * initialized. Reset all the state variables */

			bytesLeft = 0;
			eofReached = 0;
			readPtr = readBuf;
			nRead = 0;
			applicationState = FRAME_SYNC_FIND;
			break;


/*------------------------------------------------------------------------------*/
		case(FRAME_SYNC_FIND):


			/* Read the mp3 file. Fill the first buffer of
			 * data from the input file. */

			fsresult = f_read(&myfile,readBuf,READBUF_SIZE,&bytesLeft);
			if (fsresult != FR_OK){
				//File read failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning
				while(1);
			}

			/* If end of file is reached then close this file */
			if(bytesLeft == 0)
			{
				applicationState = FILE_CLOSE;
				break;
			}

			do {
			/* find start of first MP3 frame - assume
			 * EOF if no sync found */

			offset = MP3FindSyncWord(readPtr, bytesLeft);
			if (offset < 0)
			{
				/* This means this frame does not contain
				 * the frame sync. Get another frame. */

				readPtr = readBuf;
				applicationState = FRAME_SYNC_FIND;
				break;

			}

			/* Update the read pointer and number of bytes
			 * left in the current frame. */

			readPtr += offset;
			bytesLeft -= offset;

			/* Determine the audio format from the first frame header
			 * The information in mp3FrameInfo can be displayed on the
			 * display. */
			err = MP3GetNextFrameInfo(mp3Decoder, &mp3FrameInfo, readPtr);
			} while(err < 0); // as long as a valid frame was found
			// should run only once in most cases, but when ID3 tag
			// contains false SyncWord, this wil prevent error


			if( mp3FrameInfo.nChans != 2)
			{
				/* This means that the MP3 file is  a
				 * mono audio stream
				 * We currently dont want to handle these type
				 * of files even if the decoder can decode
				 * them */
				applicationState = FILE_CLOSE;
				break;
			}

			/*-----------------------------------------------------------------------------
			Initialize the audio dac and codec, with sample rate from Mp3 file
			-----------------------------------------------------------------------------*/
			EVAL_AUDIO_DeInit();
			EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO,100,mp3FrameInfo.samprate);

			/* Start reading the MP3 file.*/
			applicationState = FILE_READ;
			break;

/*------------------------------------------------------------------------------*/
		case(FILE_READ):

			/* Read some more data is the number of bytes
			 * left is less than a frame size and we have
			 * not reached the end of file.
			 */

			if (bytesLeft < READBUF_SIZE && !eofReached)
			{
				nRead = FillReadBuffer(readBuf, readPtr, READBUF_SIZE, bytesLeft, &myfile);
				bytesLeft += nRead;
				readPtr = readBuf;
				if (nRead == 0)
					eofReached = 1;
			}

			/* Decode the frame */
			applicationState = MP3_DECODE;


			break;

/*------------------------------------------------------------------------------*/
		case(MP3_DECODE):
		/*-----------------------------------------------------------------------------
		Decode mp3 file
		-----------------------------------------------------------------------------*/

			err = MP3Decode(mp3Decoder, &readPtr, (int*)&bytesLeft,(short int*) outBuf, 0);

			applicationState = PLAYBACK;
			if (err)
			{
				/* error occurred */
				switch (err)
				{
				case -1:
					applicationState = FILE_CLOSE;
					break;
				case -2:
					/* More data is needed. Read the file. */
					applicationState = FILE_READ;
					break;
				case -3:
					applicationState = FILE_CLOSE;
					break;
				case -6:
					applicationState = FRAME_SYNC_FIND;
					break;
				default:
					applicationState = FILE_CLOSE;
					break;
				}
			}

			break;

/*------------------------------------------------------------------------------*/
		case(PLAYBACK):
	            		{
			/* In this state an stereo audio frame is
			 * available to play. Play back the frame
			 * and then read the file for more
			 * data */

			EVAL_AUDIO_Play((uint16_t*)outBuf,(long int)sizeof(outBuf));

			applicationState = FILE_READ;
			break;


	            		}
		case(PLAYBACK_PAUSE):
	            				break;


		case (PLAYBACK_STOP):
	            				break;

		case (SYSTEM_HALT):
	            				break;

		case(FILE_CLOSE):
	            		{
			/* The application would have reached here if
			 * there is an error opening the file or the
			 * playback is done. In either case try the
			 * next file in the list
			 */

			EVAL_AUDIO_DeInit();
			f_close(&myfile);
			applicationState = OPEN_MP3_FILE;
			break;
	            		}
		default:
			while(1);
			break;
		}
	}

}

}









