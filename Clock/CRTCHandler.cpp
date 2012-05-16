/**
  ******************************************************************************
  * @file      	CRTCHandler.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	16.05.12
  * @brief		Class to handle the RTC and alarm requests
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CRTCHandler.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* namespace -----------------------------------------------------------------*/
namespace rtc_handler_namespace {
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

CRTCHandler::CRTCHandler() {
	// TODO Auto-generated constructor stub

}

CRTCHandler::~CRTCHandler() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  HardwareInit called before Scheduler starts
  * @param  None
  * @retval true on succsess
  */
bool CRTCHandler::HardwareInit(){


	return true;
}

/**
  * @brief  task function for Led Heartbeat
  * @param  None
  * @retval None
  */
void CRTCHandler::Run(){

}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void CRTCHandler::RTC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RTC_InitTypeDef   RTC_InitStructure;

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* LSI used as RTC source clock */
	/* The RTC Clock may varies due to LSI frequency dispersion. */
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

	/* Calendar Configuration with LSI supposed at 32KHz */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv	=  0xFF; /* (32KHz / 128) - 1 = 0xFF*/
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);

	/* EXTI configuration *******************************************************/
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable the RTC Wakeup Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	RTC_SetWakeUpCounter(0x0);

	/* Enable the RTC Wakeup Interrupt */
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* Enable Wakeup Counter */
	RTC_WakeUpCmd(ENABLE);
}

} /* namespace rtc_handler_namespace */
