/**
  ******************************************************************************
  * @file    TIM_TimeBase/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4_discovery_audio_codec.h"

#ifdef STM32_SDIO
	#include "stm32f4_sdio.h"
	#include "stm32f4xx_sdio.h"
#else
	#ifdef MEDIA_USB_KEY
		#include "usb_hcd_int.h"
		#include "usbh_usr.h"
		#include "usbh_core.h"
		#include "usbh_msc_core.h"
	#endif
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t PauseResumeStatus = 2, Count = 0, LED_Toggle = 0;
uint16_t capture = 0;
extern __IO uint8_t RepeatState;


#if defined MEDIA_USB_KEY
__IO uint16_t Time_Rec_Base = 0;
 extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
 extern __IO uint32_t XferCplt ;
 extern __IO uint8_t Command_index;
#endif /* MEDIA_USB_KEY */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}


/**
 * The following two function are handled by FreeRTOS.  See line 225
 * in port.c inside of FreeRTOS.
 */

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*void SVC_Handler(void)
{}*/

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
/*void PendSV_Handler(void)
{}*/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*void SysTick_Handler(void)
{}*/

/******************************************************************************/
/*            STM32F4xx Peripherals Interrupt Handlers                        */
/******************************************************************************/
/*
 * Sdio interface collides with I2S3 in STM32F4Discovery so
 * using USB Host MSC is the better way to get Massstorage access
 */
#ifdef STM32_SDIO
/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void SD_SDIO_DMA_IRQHANDLER(void)
{
  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
  SD_ProcessDMAIRQ();
}

#else

	/**
	  * @brief  This function handles External line 1 interrupt request.
	  *
	  * 		Triggers Event on mechanical shock, good as "Snoozze" Button :D
	  * @param  None
	  * @retval None
	  */
	void EXTI1_IRQHandler(void)
	{
	  /* Check the clic on the accelerometer to Pause/Resume Playing */
	  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	  {
		/*give sem in FreeRTOS */


		/* Clear the EXTI line 1 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line1);
	  }
	}


	#if defined MEDIA_USB_KEY
		/**
		  * @brief  EXTI0_IRQHandler
		  *         This function handles External line 0 interrupt request.
		  * @param  None
		  * @retval None
		  */
		void EXTI0_IRQHandler(void)
		{
		  /* Checks whether the User Button EXTI line is asserted*/
		  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
		  {
#ifdef test
			if (Command_index == 1)
			{
			  /* Switch to play command */
			  Command_index = 0;
			}
			else if (Command_index == 0)
			{
			  /* Switch to record command */
			  Command_index = 1;
			  XferCplt = 1;
			  EVAL_AUDIO_Stop(CODEC_PDWN_SW);
			}
			else
			{
			  /* Switch to play command */
			  Command_index = 0;
			}
#endif
		  }

		  /* Clears the EXTI's line pending bit.*/
		  EXTI_ClearITPendingBit(EXTI_Line0);
		}


		/**
		  * @brief  This function handles USB-On-The-Go FS global interrupt request.
		  * @param  None
		  * @retval None
		  */
		void OTG_FS_IRQHandler(void)
		{
		  USBH_OTG_ISR_Handler(&USB_OTG_Core);
		}

	#endif /* MEDIA_USB_KEY */


#endif //STM32_SDIO


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
