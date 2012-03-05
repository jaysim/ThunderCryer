// STM32F10 SDIO_DRIVE driver written by J. Shaler

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

#include "stm32f4_sdio.h"   // for SDIO peripheral device implementation
#include "fattime.h" // for get_fattime implementation


/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
#define SDIO_DRIVE		0




// Private variables
SD_CardInfo SDCardInfo;



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */

DSTATUS disk_initialize (
                         BYTE drv				/* Physical drive nmuber (0..) */
                           )
{ 
NVIC_InitTypeDef NVIC_InitStructure;
SD_Error Status;

	switch (drv)
	{
	case SDIO_DRIVE:

		// SDIO Interrupt ENABLE
		NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		// DMA2 STREAMx Interrupt ENABLE
		NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_Init(&NVIC_InitStructure);


		/*-------------------------- SD Init ----------------------------- */
		Status = SD_Init();


		if (Status == SD_OK)
		{
			//SD Card initialized ok.
			/*----------------- Read CSD/CID MSD registers ------------------*/
			Status = SD_GetCardInfo(&SDCardInfo);
		}


		if (Status == SD_OK);
			return 0x00;

		break;
	}
	return STA_NOINIT;
}






/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
                     BYTE drv		/* Physical drive nmuber (0..) */
                       )
{
  switch (drv) 
  {
    case SDIO_DRIVE:
    {
      SD_Error status = SD_GetCardInfo(&SDCardInfo);
      
      if (status != SD_OK)
        return STA_NOINIT;
      else
        return 0x00;
    }
  }
  
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
                   BYTE drv,		/* Physical drive nmuber (0..) */
                   BYTE *buff,		/* Data buffer to store read data */
                   DWORD sector,	/* Sector address (LBA) */
                   BYTE count		/* Number of sectors to read (1..255) */
                     )
{
	SD_Error status = SD_OK;

	switch (drv)
	{
	case SDIO_DRIVE:
		status = SD_ReadMultiBlocks((uint8_t*)buff,sector*512,512,count);

		/* Check if the Transfer is finished */
		status = SD_WaitReadOperation();

		/* Wait until end of DMA transfer */
		while(SD_GetStatus() != SD_TRANSFER_OK);

		if (status == SD_OK){
			return RES_OK;
		}  else {
			return RES_ERROR;
		}
		break;
	}
  return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
                    BYTE drv,			/* Physical drive nmuber (0..) */
                    const BYTE *buff,	/* Data to be written */
                    DWORD sector,		/* Sector address (LBA) */
                    BYTE count			/* Number of sectors to write (1..255) */
                      )
{
	SD_Error status = SD_OK;
	switch (drv)
	{
	case SDIO_DRIVE:
		status = SD_WriteMultiBlocks((uint8_t*)(buff),sector*512,512,count);

		/* Check if the Transfer is finished */
		status = SD_WaitWriteOperation();

		/* Wait until end of DMA transfer */
		while(SD_GetStatus() != SD_TRANSFER_OK);

		if (status == SD_OK){
			return RES_OK;
		}  else {
			return RES_ERROR;
		}
		break;
	}
	return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
                    BYTE drv,		/* Physical drive nmuber (0..) */
                    BYTE ctrl,		/* Control code */
                    void *buff		/* Buffer to send/receive control data */
                      )
{
  switch (drv) 
  {
    case SDIO_DRIVE:
    {      
      switch (ctrl)
      {
        case CTRL_SYNC:
          // no synchronization to do since not buffering in this module
          return RES_OK;
        case GET_SECTOR_SIZE:
          *(WORD*)buff = 512;
          return RES_OK;
        case GET_SECTOR_COUNT:
          *(DWORD*)buff = SDCardInfo.CardCapacity / 512;
          return RES_OK;
        case GET_BLOCK_SIZE:
          *(DWORD*)buff = 512;
          return RES_OK;
          break;
      }
      break;
    }
  }
  return RES_PARERR;
}

