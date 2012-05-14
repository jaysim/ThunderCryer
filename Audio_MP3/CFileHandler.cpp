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
#include "stm32f4_discovery_lis302dl.h"
#include "ctype.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static const UINT READBUF_SIZE=MAINBUF_SIZE;
static const UINT READBUF_THRESHOLD=MAINBUF_SIZE/2-1;	// smaller then half of READBUF_SIZE to ensure no overlap for RefillBuffer
static const UINT PCM_OUT_SIZE = MAX_NGRAN * MAX_NGRAN * MAX_NSAMP; // max Output from MP3 decoder
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CUSB_MassStorage g_MSC;
xSemaphoreHandle semI2SDMAFinished;
xSemaphoreHandle semUserButton;
xSemaphoreHandle semShock;

static uint8_t  uiReadBuffer[READBUF_SIZE];
static int16_t  iPCMBuffer1[PCM_OUT_SIZE]= {0x0000}; //double Buffering
static int16_t  iPCMBuffer2[PCM_OUT_SIZE] = {0x0000}; //double Buffering
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CFileHandler::CFileHandler() {
	fsresult = FR_OK;
	uiBytesLeft = 0;
	ptrReadPosition = uiReadBuffer;
	eMP3State = OPEN_FILE;
	eBuffer = BUFFER_1;
	uiVolume = 80;
	uiLastSamplerate = 44100;
	bPlaying  = true;
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
	vSemaphoreCreateBinary(semShock);
	vSemaphoreCreateBinary(semUserButton);
	// ensure that the semaphores are taken
	xSemaphoreTake(semShock,0);
	xSemaphoreTake(semUserButton,0);

	/*
	 * configure onboard accelerometer to drive
	 * an external interrupt line on mechanical shock
	 */
	Mems_Config();
	/*
	 * configure Interrupt for UserButton
	 */
	EXTILine_Config();

	hMP3Decoder = MP3InitDecoder();
	/*
	 * first Codec init with standard samplerate
	 */
	EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO,uiVolume,uiLastSamplerate);
	// stop I2S to prevent noise
	EVAL_AUDIO_Stop(CODEC_PDWN_SW);
	return true;
}

/**
  * @brief  task function for file handling
  * @param  None
  * @retval None
  */
void CFileHandler::Run(){
	vSemaphoreCreateBinary(semI2SDMAFinished);

	/*
	 * Read configuration from file on startup
	 */
	ReadConfig();

	/*
	 * start mp3 player, will not leave this function
	 */
	MP3Player();

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
  * @brief  reads configuration from filesystem
  *
  */
void CFileHandler::ReadConfig(){

}

/**
  * @brief  writes configuration to filesystem
  *
  */
void CFileHandler::WriteConfig(){
}

/**
  * @brief  MP3 player application
  * 		Playes MP3 from he whole device
  */
void CFileHandler::MP3Player(){
	int i;
	bool bFileIsMP3 = false;

	//operate files only when device connected
	while(GetUSBConnected()){

		switch(ePlayerState){
/*------------------------------------------------------------------------------*/
		case OPEN_DIR:

			/* Open the directory */
			fsresult = f_opendir(&dir, path);
			if (fsresult == FR_OK) {
				/* get new file on success */
				ePlayerStatePrev = ePlayerState;
				ePlayerState = GET_FILE;

				if(bStop){
					ePlayerState = STOP;
					bStop = false;
				}
			}
			break;
/*------------------------------------------------------------------------------*/
		case GET_FILE:
			/* Read a directory item */
			fsresult = f_readdir(&dir, &fileInfo);
			if (fsresult != FR_OK || fileInfo.fname[0] == 0){
				break;  /* Break on error or end of dir */
			}
			if (fileInfo.fname[0] == '.'){
				continue;             /* Ignore dot entry */
			}
			if (fileInfo.fattrib & AM_DIR) {
				/* It is a directory */
				/* copy dir Path in Path var */
				strcpy(path,fileInfo.fname);
				/* open the new Folder */
				ePlayerStatePrev = ePlayerState;
				ePlayerState = OPEN_DIR;

				/* get next file on next flag */
				if(bNext) {
					ePlayerState = GET_FILE;
					bNext = false;
				}

				if(bStop){
					ePlayerState = STOP;
					bStop = false;
				}
			} else {
				/* It is a file. */
				bFileIsMP3 = false;
				/* check filetype */
				for(i=sizeof(fileInfo.fname)-4;i>=0;i--){
					if(fileInfo.fname[i] == '.'){
						if((toupper(fileInfo.fname[i+1]) == 'M') &&
							(toupper(fileInfo.fname[i+2]) == 'P') &&
							((fileInfo.fname[i+3]) == '3')){
							/* Fileending is .MP3, so play it */
							bFileIsMP3 = true;
							break;
						}
					}
				}
				ePlayerStatePrev = ePlayerState;
				/* check if its an MP3 */
				if(bFileIsMP3){
					/* play MP3 */
					ePlayerState = PLAY_FILE;

					/* get next file on next flag */
					if(bNext) {
						ePlayerState = GET_FILE;
						bNext = false;
					}

				} else {
					/*get next file*/
					ePlayerState = GET_FILE;

					if(bStop){
						ePlayerState = STOP;
						bStop = false;
					}

				}
			}

			break;
/*------------------------------------------------------------------------------*/
		case PLAY_FILE:

			PlayMP3(fileInfo.fname);

			ePlayerStatePrev = ePlayerState;

			/* get next file on next flag */
			if(bNext) {
				ePlayerState = GET_FILE;
				bNext = false;
			}

			if(bStop){
				ePlayerState = STOP;
				bStop = false;
			}

			break;
/*------------------------------------------------------------------------------*/
		case STOP:
			/* start playing again */
			if(bPlay){
				/*check for stop loop */
				if(ePlayerStatePrev == STOP)
					ePlayerStatePrev = OPEN_DIR;
				/* go back to previous state */
				ePlayerState = ePlayerStatePrev;
				ePlayerStatePrev = STOP;
				/*clear Flag*/
				bPlay = false;
			}
			break;
/*------------------------------------------------------------------------------*/
		default:
			ePlayerStatePrev = ePlayerState;
			ePlayerState = OPEN_DIR;
			break;
		}
	}
}

/**
  * @brief  gets sem from USB handling layer
  *
  * @retval true for success and false for fault
  */
bool CFileHandler::GetUSBRelease(portTickType delay){
	/*
	 * get sem from USB Application layer
	 * to process without disturbing USB handling
	 */
	xSemaphoreTake(semUSBApplication,delay);

	/*
	 * check if Device is Connected
	 */
	if(GetUSBConnected()){
		return true;
	} else {
		/*
		 * Release Sem from USB
		 */
		ReleaseUSB();
		return false;
	}

}

/**
  * @brief  gets connected status from USB
  *
  * @retval true for connected and false for not
  */
inline bool CFileHandler::GetUSBConnected(){
	return g_MSC.IsDeviceConnected();
}


/**
  * @brief  give USB sem Back to USB handling Layer
  */
void CFileHandler::ReleaseUSB(){
	/*
	 * Application layer needs to give back the sem when ever possible
	 */
	xSemaphoreGive(semUSBApplication);
}

/**
  * @brief  plays mp3 file
  *
  * @retval true for success and false for fault
  */
bool CFileHandler::PlayMP3(const char* filename){
	static int offset = 0;
	static int err = 0;

	//operate files only when device connected
	while(GetUSBConnected()){

		/* check if file is skipped */
		if(bNext) {
			/* file is to be skiped */
			return false;
		}

		switch(eMP3State){
/*------------------------------------------------------------------------------*/
		case OPEN_FILE:

			fsresult = f_open(&file,filename,FA_OPEN_EXISTING | FA_READ);
			if(fsresult != FR_OK)
				break;

			f_sync(&file); //see app note from chan

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
				if(uiLastSamplerate != mp3FrameInfo.samprate) {
					/* Initialize I2S interface */
					EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_I2S);
					/*
					 * initialize Codec with sample freq.
					 */
					portENTER_CRITICAL();
					EVAL_AUDIO_DeInit();
					EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO,uiVolume,mp3FrameInfo.samprate);
					portEXIT_CRITICAL();
					uiLastSamplerate = mp3FrameInfo.samprate;
				}
				/*
				 * Frame is valid initialize Codec is initialized with samperate
				 * so lets go playing mp3
				 */
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

			if(eBuffer == BUFFER_1){
				err = MP3Decode(hMP3Decoder,&ptrReadPosition,(int*)&uiBytesLeft,iPCMBuffer1,0);
			}else{
				err = MP3Decode(hMP3Decoder,&ptrReadPosition,(int*)&uiBytesLeft,iPCMBuffer2,0);
			}

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
				if(eBuffer == BUFFER_1) {
					EVAL_AUDIO_Play(iPCMBuffer1,sizeof(iPCMBuffer1));
					eBuffer = BUFFER_2;
				}else{
					EVAL_AUDIO_Play(iPCMBuffer2,sizeof(iPCMBuffer2));
					eBuffer = BUFFER_1; //switch Buffers
				}
				eMP3State = DECODE;
			}

			break;

		default:
			// stop I2S to prevent noise
			EVAL_AUDIO_Stop(CODEC_PDWN_SW);
			fsresult = FR_OK;
			uiBytesLeft = 0;
			ptrReadPosition = uiReadBuffer;
			eBuffer = BUFFER_1;
			/*
			 * Close-File State not implemented,
			 * all unknown states result in file close
			 */
			f_close(&file);
			eMP3State = OPEN_FILE;
			return true;
			break;
		}
	}
	eMP3State = OPEN_FILE;
	return false; //Device disconnected
}

/**
  * @brief  plays next song
  *
  */
inline void CFileHandler::NextSong(){
	/* the flag will be cleared when Song was skipped */
	bNext = true;
}

/**
 * @brief halts MP3Player
 */
inline void CFileHandler::StopPlayer(){
	bStop = true;
}

/**
 * @brief starts MP3Player
 */
inline void CFileHandler::StartPlayer(){
	bPlay = true;
}

/**
 * @brief get the actual filename of the song played
 * @return filename of played song
 */
const char* CFileHandler::GetSong(){
	return fileInfo.fname;
}

/**
  * @brief  toggles play, pause state
  *
  */
void CFileHandler::PlayPause(){
	if(bPlaying) {
		EVAL_AUDIO_PauseResume(AUDIO_PAUSE);
		bPlaying = false;
	} else {
		EVAL_AUDIO_PauseResume(AUDIO_RESUME);
		bPlaying = true;
	}
}



/**
  * @brief  set actual DAC volume in %
  *
  */
void CFileHandler::SetVolume(uint8_t newVolume){
	uiVolume = newVolume;
	EVAL_AUDIO_VolumeCtl(newVolume);
}


/**
 * @brief  configure the mems accelometer
 * @param  None
 * @retval None
 */
void CFileHandler::Mems_Config(void)
{
	uint8_t ctrl = 0;

	LIS302DL_InitTypeDef  LIS302DL_InitStruct;
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;

	/* Set configuration of LIS302DL*/
	LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
	LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	LIS302DL_Init(&LIS302DL_InitStruct);

	/* Set configuration of Internal High Pass Filter of LIS302DL*/
	LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
	LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);

	/* Configure Interrupt control register: enable Click interrupt on INT1 and
     INT2 on Z axis high event */
	ctrl = 0x3F;
	LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);

	/* Enable Interrupt generation on click on Z axis */
	ctrl = 0x50;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

	/* Configure Click Threshold on X/Y axis (10 x 0.5g) */
	ctrl = 0xAA;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);

	/* Configure Click Threshold on Z axis (10 x 0.5g) */
	ctrl = 0x0A;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);

	/* Enable interrupt on Y axis high event */
	ctrl = 0x4C;
	LIS302DL_Write(&ctrl, LIS302DL_FF_WU_CFG1_REG_ADDR, 1);

	/* Configure Time Limit */
	ctrl = 0x03;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);

	/* Configure Latency */
	ctrl = 0x7F;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);

	/* Configure Click Window */
	ctrl = 0x7F;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1);

}

/**
 * @brief  Configures EXTI Line0 (connected to PA0 pin) in interrupt mode
 * @param  None
 * @retval None
 */
void CFileHandler::EXTILine_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	/* Enable GPIOA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Configure PE0 and PE1 pins as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Connect EXTI Line to PE1 pins */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable and set EXTI Line0 Interrupt to the priority 1 higher than RTOS Kern */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY-1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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








