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
#include "string.h"
#include "stm32f4_discovery_audio_codec.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static const UINT READBUF_SIZE=MAINBUF_SIZE;
static const UINT READBUF_THRESHOLD=MAINBUF_SIZE/2-1;	// smaller then half of READBUF_SIZE to ensure no overlap for RefillBuffer
static const UINT PCM_OUT_SIZE = MAX_NGRAN * MAX_NGRAN * MAX_NSAMP; // max Output from MP3 decoder
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CUSB_MassStorage g_MSC;
xSemaphoreHandle semI2SDMAFinished;

static uint8_t  uiReadBuffer[READBUF_SIZE];
static uint16_t  uiPCMBuffer[2][PCM_OUT_SIZE]; //double Buffering
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CFileHandler::CFileHandler() {
	fsresult = FR_OK;
	uiBytesLeft = 0;
	ptrReadPosition = uiReadBuffer;
	eMP3State = OPEN_FILE;
	eBuffer = BUFFER_1;
	uiVolume = 100;

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
	hMP3Decoder = MP3InitDecoder();
	return true;
}

/**
  * @brief  task function for Led Heartbeat
  * @param  None
  * @retval None
  */
void CFileHandler::Run(){
	vSemaphoreCreateBinary(semI2SDMAFinished);

	while(1){
		/*
		 * get sem from USB Application layer
		 * to precess without disturbing USB
		 */
		xSemaphoreTake(semUSBApplication,portMAX_DELAY);

		PlayMP3("MacGyver.mp3");

		/*
		 *Application layer needs to give back the sem when ever possible
		 */
		xSemaphoreGive(semUSBApplication);

	}
}



/**
  * @brief  Refills the Decoder Buffer
  *
  * @retval true for success and false for fault
  */
bool CFileHandler::RefillBuffer(void){
	UINT uiBytesToRead=0;
	UINT uiBytesRead=0;

	if(uiBytesLeft > 0) {
		// there are read bytes in buffer
		// need to move them to the start
		//
		// Fat reading with offset is too slow
		// memmove is faster
		//
		// enshure there is no overlap
		if(uiBytesLeft > READBUF_THRESHOLD )
			return false; // illegal call of RefillBuffer

		portENTER_CRITICAL();	// prevent RTOS from scheduling
		memmove(uiReadBuffer,ptrReadPosition,uiBytesLeft);
		portEXIT_CRITICAL();
	}

	uiBytesToRead = READBUF_SIZE - uiBytesLeft;

	fsresult = f_read(&file,uiReadBuffer+uiBytesLeft,uiBytesToRead,&uiBytesRead);
	if(fsresult != FR_OK)
		return false; // Readerror

	uiBytesLeft += uiBytesRead; // add newly read bytes
	ptrReadPosition = uiReadBuffer; // set read position to beginning of buffer

	return true;
}

/**
  * @brief  plays mp3 file
  *
  * @retval true for success and false for fault
  */
bool CFileHandler::PlayMP3(const char * filename){
	int offset = 0;
	int err = 0;

	//operate files only when device connected
	while(g_MSC.IsDeviceConnected()){

		switch(eMP3State){
/*------------------------------------------------------------------------------*/
		case OPEN_FILE:

			fsresult = f_open(&file,filename,FA_OPEN_EXISTING | FA_READ);
			if(fsresult != FR_OK)
				break;

			if(!RefillBuffer()) // read data from file
				break;

			eMP3State = FIND_SYNC;

			break;
/*------------------------------------------------------------------------------*/
		case FIND_SYNC:
			// search beginning of MP3 data
			offset = MP3FindSyncWord(ptrReadPosition, uiBytesLeft);
			if (offset < 0) {
				// no Sync found in Buffer
				uiBytesLeft = 0; // discard all data
				RefillBuffer(); // read data from file
				break;
			}
			// move read ptr to Frame position
			ptrReadPosition += offset;
			uiBytesLeft -= offset;

			/*
			 * ensure that the found Frame is a valid frame and
			 * not some weird ID3 tag
			 */
			err = MP3GetNextFrameInfo(hMP3Decoder,&mp3FrameInfo,ptrReadPosition);
			if(err == 0 && mp3FrameInfo.nChans == 2 && mp3FrameInfo.version == 0){
				/*
				 * Frame is valid initialize Codec with sample freq.
				 */
				portENTER_CRITICAL();
				//EVAL_AUDIO_DeInit();
				EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO,uiVolume,mp3FrameInfo.samprate);
				portEXIT_CRITICAL();

				eMP3State = DECODE;
			}else if(err == 0){
				/*
				 * valid frame but not compatible with the codec
				 */
				eMP3State = CLOSE_FILE;
			}

			break;
/*------------------------------------------------------------------------------*/
		case DECODE:

			if(uiBytesLeft < READBUF_THRESHOLD) {
				if(!RefillBuffer()) // end of file
					eMP3State = CLOSE_FILE;
			}

			err = MP3Decode(hMP3Decoder,&ptrReadPosition,(int*)&uiBytesLeft,(short int*)uiPCMBuffer[eBuffer],0);
			if (err)
			{
				/* error occurred */
				switch (err)
				{
				case ERR_MP3_INDATA_UNDERFLOW:
					if(!RefillBuffer()){ // end of file
						eMP3State = CLOSE_FILE;
					}
					break;
				case ERR_MP3_MAINDATA_UNDERFLOW:
					eMP3State = DECODE; // more data needed
					break;
				case ERR_MP3_FREE_BITRATE_SYNC:
					eMP3State = CLOSE_FILE;
					break;
				case ERR_MP3_INVALID_FRAMEHEADER:
					eMP3State = FIND_SYNC;
					break;
				default:
					eMP3State = CLOSE_FILE;
					break;
				}
			} else {
				eMP3State = PLAYBACK;
			}

			break;

/*------------------------------------------------------------------------------*/
		case PLAYBACK:
			if(xSemaphoreTake(semI2SDMAFinished , portMAX_DELAY ) == pdTRUE){
				/*
				 * start playing the samples
				 */
				EVAL_AUDIO_Play(uiPCMBuffer[eBuffer],mp3FrameInfo.outputSamps);
				if(eBuffer == BUFFER_1)
					eBuffer = BUFFER_2;
				else
					eBuffer = BUFFER_1; //switch Buffers

				eMP3State = DECODE;
			}

			break;

		default:
			/*
			 * Close-File State not implemented,
			 * all unknown states result in file close
			 */
			f_close(&file);
			eMP3State = OPEN_FILE;
			return false;
			break;
		}
	}
	eMP3State = OPEN_FILE;
	return false; //Device disconnected
}


extern "C" {
/*--------------------------------
Callbacks implementation:
the callbacks prototypes are defined in the stm324xg_eval_audio_codec.h file
and their implementation should be done in the user code if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/
/**
 * @brief  Calculates the remaining file size and new position of the pointer.
 * @param  None
 * @retval None
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
	/* Calculate the remaining audio data in the file and the new size
  for the DMA transfer. If the Audio files size is less than the DMA max
  data transfer size, so there is no calculation to be done, just restart
  from the beginning of the file ... */
	/* Check if the end of file has been reached */

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(semI2SDMAFinished,&xHigherPriorityTaskWoken);

	/*
	 * triggers PendSV handler for context switch
	 * but when DMA Handler has higher Priority
	 * this has no effect until ISR is finished
	 */
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


/**
 * @brief  Get next data sample callback
 * @param  None
 * @retval Next data sample to be sent
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
	return 0;
}

}
}








