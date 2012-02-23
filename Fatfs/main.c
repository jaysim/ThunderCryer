/**
  ******************************************************************************
  * @file    SDIO/main.c 
  * @author  MCD Application Team, modified by Albert McGilvra for SD_FAT project.
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sdcard.h"

//mcg added includes
#include "ff.h"
#include <stdio.h>
#include <cross_studio_io.h>
#include <string.h>
#include "timestamp.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SDIO_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks) >> 2)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SD_CardInfo SDCardInfo;
SD_Error Status = SD_OK;

uint32_t Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
uint32_t Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Clock Config */
  RCC_Configuration();

  /* Interrupt Config */
  NVIC_Configuration();

  debug_printf("\n");
  /*-------------------------- SD Init ----------------------------- */
  Status = SD_Init();

  if (Status == SD_OK)
  {
    debug_printf("SD Card initialized ok.\n");
   /*----------------- Read CSD/CID MSD registers ------------------*/
    Status = SD_GetCardInfo(&SDCardInfo);
  }
  else
  {
    debug_printf("SD Card did not initialize, check that a card is inserted. SD_Error code: %d.  See sdcard.h for SD_Error code meaning.\n", Status);
    while(1);  //infinite loop
  }

  if (Status == SD_OK)
  {
    debug_printf("SD Card information retrieved ok.\n");
    /*----------------- Select Card --------------------------------*/
    Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
  }
  else
  {
    debug_printf("Could not get SD Card information. SD_Error code: %d.  See sdcard.h for SD_Error code meaning.\n", Status);
    while(1);  //infinite loop
  }

  if (Status == SD_OK)
  {
    debug_printf("SD Card selected ok.\n");
   /*----------------- Enable Wide Bus Operation --------------------------------*/
    Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
  }
  else
  {
    debug_printf("SD Card selection failed. SD_Error code: %d.  See sdcard.h for SD_Error code meaning.\n", Status);
    while(1);  //infinite loop
  }

  if (Status == SD_OK)
     debug_printf("SD Card 4-bit Wide Bus operation successfully enabled.\n");
  else
  {
    debug_printf("Could not enable SD Card 4-bit Wide Bus operation, will revert to 1-bit operation.\nSD_Error code: %d.  See sdcard.h for SD_Error code meaning.\n", Status);
  }

/*-----------------------------------------------------------------------------
Beginning of FAT file system related code.  The following code shows steps 
necessary to create, read, and write files.  
See http://elm-chan.org/fsw/ff/00index_e.html for more information.
This code assumes a single logical drive, drive number 0.  It also assumes a single partition.
-----------------------------------------------------------------------------*/

  FRESULT fsresult;               //return code for file related operations
  FATFS myfs;                     //FAT file system structure, see ff.h for details
  FIL myfile;                     //file object

/*-----------------------------------------------------------------------------
Mount the file system on logical drive 0.  Mounting associates the file system 
structure with a logical drive number.  Mounting must be done before any file 
operations.  Mounting does not write anything to the SD card, it simply 
initializes and associates the file structure.  The file system structure 
members are not filled in until the first file operation after f_mount.
-----------------------------------------------------------------------------*/
  fsresult = f_mount(0, &myfs);   
  if (fsresult == FR_OK)
     debug_printf("FAT file system mounted ok.\n");
  else
    debug_printf("FAT file system mounting failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

/*-----------------------------------------------------------------------------
Format the SD card.  The f_mkfs makes a file system on the SD card which is
the same as formatting it.  The arguments are drive #, partition type, and
cluster size.  The drive number is 0 here, the partition type is 0 for FDISK,
and the cluster size is zero for auto-select.  Auto-select selects a cluster
size based on drive size.  The FAT sub-type (FAT12/16/32) is not explicitly 
specified but is chosen according to the number of clusters.  The file system 
structure has members that indicate FAT12/16/32, as well as the cluster size.
-----------------------------------------------------------------------------*/
/* vvv Comment out this block to prevent formatting.  Delete/insert '/' at the end of this line: */
  debug_printf("Formatting SD Card. Please wait... ");
  fsresult = f_mkfs(0, 0, 0);  //format drive 0 with FDISK partitioning rule, auto-select cluster size by volume size.
  if (fsresult == FR_OK)  
    debug_printf("Format successful.\n");
  else
    debug_printf("Format failed.  FRESULT Error code: %d.  See FATfs/ff.h for FRESEULT code meaning.\n", fsresult);
/* ^^^ Comment out this block to prevent formatting. */


/*-----------------------------------------------------------------------------
Open a file, write to it, and close it.
-----------------------------------------------------------------------------*/
  fsresult = f_open(&myfile, "hello.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);  //open/create file for read/write 
  if (fsresult == FR_OK)
     debug_printf("File opened ok.\n");
  else
    debug_printf("File open failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

  BYTE wbuf[16];  //buffer to write to file
  UINT numwritten; //number of bytes actually written
  strcpy(wbuf, "hello world");
  fsresult = f_write(&myfile, wbuf, strlen(wbuf), &numwritten);  //open/create file for read/write 
  if (fsresult == FR_OK)
     debug_printf("File write ok, %d bytes written.\n", numwritten);
  else
    debug_printf("File write failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

  fsresult = f_close(&myfile);
  if (fsresult == FR_OK)
     debug_printf("File closed ok.\n");
  else
    debug_printf("File close failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

/*-----------------------------------------------------------------------------
Print some card info.  This is done after the open/write/close sequence
because at least one file operation must occur after the f_mount to fill in the 
file system structure (myfs) member values.
-----------------------------------------------------------------------------*/
// Determine FAT sub-type
  BYTE fssubtype;
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
  } 
  debug_printf("File system type is FAT%d.\n", fssubtype);
  debug_printf("Sector size = %d bytes.  Sectors are physical blocks.\n", SS(fs));
  debug_printf("Cluster size = %d sectors.  Clusters are logical file system blocks.\n", myfs.csize); 
  debug_printf("Card capacity %ld KB.\n", SDCardInfo.CardCapacity / 1024); 
  debug_printf("Card block size %ld bytes.\n", SDCardInfo.CardBlockSize);
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
    debug_printf("%lu KB free space. %lu KB total.\n",   fre_bytes / 1024, tot_bytes / 1024);  
  }
  else
    debug_printf("f_getfree failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);
  
/*-----------------------------------------------------------------------------
Set file date and time.  The FILINFO structure used to hold file information.  
The time and date members are used by f_utime to set the file time and date.  
The other members are irrelevant for setting time and date.  
f_stat and f_readdir fill in members of the FILINFO structure, see
http://elm-chan.org/fsw/ff/00index_e.html for more information.
-----------------------------------------------------------------------------*/  
  FILINFO myfileinfo;  
                 //  0bYYYYYYYMMMMDDDDD
  myfileinfo.fdate = 0b0011101100101010;  //date is packed in high order word, Sept 10, 2009
  myfileinfo.ftime = 0x0000;  //time is packed in low order word, 00:00:00

  fsresult = f_utime("hello.txt", &myfileinfo);
  if (fsresult == FR_OK)
     debug_printf("File date/time set ok.\n");
  else
    debug_printf("File date/time set failed. FRESULT Error code: %d.  See FATfs/ff.h for FRESULT code meaning.\n", fsresult);

/* Infinite loop */
  while (1)
  {} 
}
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
