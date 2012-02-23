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
#include "ff.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



CFileHandler::CFileHandler() {
	Status = SD_OK;
	fsresult = FR_OK;
}

CFileHandler::~CFileHandler() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * 		SD Card interface will be initialised
  * @param  None
  * @retval true on succsess
  */
bool CFileHandler::HardwareInit(){

	/*-------------------------- SD Init ----------------------------- */
	Status = SD_Init();

	if (Status == SD_OK)
	{
		//SD Card initialized ok.
		/*----------------- Read CSD/CID MSD registers ------------------*/
		Status = SD_GetCardInfo(&SDCardInfo);
	}

	return (Status == SD_OK);
}

/**
  * @brief  task function for Led Heartbeat
  * @param  None
  * @retval None
  */
void CFileHandler::Run(){
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

	if (fsresult != FR_OK)
		//FAT file system mounting failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.
		while(1);

	/*-----------------------------------------------------------------------------
	Format the SD card.  The f_mkfs makes a file system on the SD card which is
	the same as formatting it.  The arguments are drive #, partition type, and
	cluster size.  The drive number is 0 here, the partition type is 0 for FDISK,
	and the cluster size is zero for auto-select.  Auto-select selects a cluster
	size based on drive size.  The FAT sub-type (FAT12/16/32) is not explicitly
	specified but is chosen according to the number of clusters.  The file system
	structure has members that indicate FAT12/16/32, as well as the cluster size.
	-----------------------------------------------------------------------------*/
	/* vvv Comment out this block to prevent formatting.  Delete/insert '/' at the end of this line: *
	printf("Formatting SD Card. Please wait... ");
	fsresult = f_mkfs(0, 0, 0);  //format drive 0 with FDISK partitioning rule, auto-select cluster size by volume size.
	if (fsresult == FR_OK)
		printf("Format successful.\n");
	else
		printf("Format failed.  FRESULT Error code: %d.  See FATfs/ff.h for FRESEULT code meaning.\n", fsresult);
	* ^^^ Comment out this block to prevent formatting. */


	/*-----------------------------------------------------------------------------
	Open a file, write to it, and close it.
	-----------------------------------------------------------------------------*/
	fsresult = f_open(&myfile, "hello.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);  //open/create file for read/write
	if (fsresult != FR_OK)
		//File open failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning
		while(1);

	BYTE wbuf[16];  //buffer to write to file
	UINT numwritten; //number of bytes actually written
	strcpy((char*)wbuf, "hello world");


	fsresult = f_write(&myfile, wbuf, strlen((const char*)wbuf), &numwritten);  //open/create file for read/write
	if (fsresult != FR_OK)
		while(1);
		//File write failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning

	fsresult = f_close(&myfile);
	if (fsresult != FR_OK)
		while(1);
		//File close failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning

	/*-----------------------------------------------------------------------------
	Print some card info.  This is done after the open/write/close sequence
	because at least one file operation must occur after the f_mount to fill in the
	file system structure (myfs) member values.
	-----------------------------------------------------------------------------*/
	// Determine FAT sub-type
	/*BYTE fssubtype;
	switch (myfs.fs_type)
	{
	case 1:
		fssubtype = 12;
		break;
	case 2:
		fssubtype = 16;
		break;
	case 3:
		fssubtype = 32;
		break;
	}*/

	/*
	printf("File system type is FAT%d.\n", fssubtype);
	printf("Sector size = %d bytes.  Sectors are physical blocks.\n", SS(fs));
	printf("Cluster size = %d sectors.  Clusters are logical file system blocks.\n", myfs.csize);
	printf("Card capacity %ld KB.\n", SDCardInfo.CardCapacity / 1024);
	printf("Card block size %ld bytes.\n", SDCardInfo.CardBlockSize);
	// Get drive information and free clusters
	uint32_t fre_clust, tot_bytes, fre_bytes;
	FATFS *myfsptr = &myfs;  //this is necessary because f_getfree expects a pointer to a pointer to a file system object.
	fsresult = f_getfree("/", &fre_clust, &myfsptr);
	if (fsresult == FR_OK)
	{
		// Get total bytes and free bytes
		tot_bytes = (myfs.max_clust - 2) * myfs.csize * SS(fs);
		fre_bytes = fre_clust * myfs.csize * SS(fs);
		// Print free space in unit of KB
		printf("%lu KB free space. %lu KB total.\n",   fre_bytes / 1024, tot_bytes / 1024);
	}
	else
		printf("f_getfree failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

	*-----------------------------------------------------------------------------
		Set file date and time.  The FILINFO structure used to hold file information.
		The time and date members are used by f_utime to set the file time and date.
		The other members are irrelevant for setting time and date.
		f_stat and f_readdir fill in members of the FILINFO structure, see
		http://elm-chan.org/fsw/ff/00index_e.html for more information.
		-----------------------------------------------------------------------------*/
	/*FILINFO myfileinfo;
	//  0bYYYYYYYMMMMDDDDD
	myfileinfo.fdate = 0b0011101100101010;  //date is packed in high order word, Sept 10, 2009
	myfileinfo.ftime = 0x0000;  //time is packed in low order word, 00:00:00

	fsresult = f_utime("hello.txt", &myfileinfo);
	if (fsresult == FR_OK)
		printf("File date/time set ok.\n");
	else
		printf("File date/time set failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

	* Infinite loop */
	while (1)
	{}
}}


