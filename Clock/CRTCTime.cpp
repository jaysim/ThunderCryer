/**
 ******************************************************************************
 * @file      	CRTCTime.cpp
 * @author    	Tecnologic86
 * @version   	V0.0.0
 * @date      	11.07.2012 12:39:40
 * @project	ThunderCryer
 ******************************************************************************
 */
#include "CRTCTime.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"

CRTCTime::CRTCTime() {
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv	=  0xFF; /* (32KHz / 128) - 1 = 0xFF*/
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
}

CRTCTime::~CRTCTime() {
}

/**
 * Initializes the RTC with the predefined values (see Constructors)
 * only call in HardwareInitfunction
 */
void CRTCTime::Init(){

	/*
	 * enable Pwr domain clock
	 */
	RCC_AHB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);

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
	RTC_Init(&RTC_InitStructure);


}



/**
 * set time in RTC registers to actual value
 */
void CRTCTime::SetTimeInRTC(){
	struct std::tm* newTime = 0;

	/*
	 * Lock RTCAccess to
	 */
	RTC_Mutex.Take(portMAX_DELAY);

	/*
	 * convert t_time to tm
	 */
	newTime = std::gmtime(&_Time);

	/*
	 * set RTC date struct
	 */
	RTC_DateStructure.RTC_Date = newTime->tm_mday;
	RTC_DateStructure.RTC_Month = newTime->tm_mon;
	RTC_DateStructure.RTC_Year = newTime->tm_year + 1900;

	/*
	 * set RTC Time struct
	 */
	RTC_TimeStructure.RTC_Seconds = newTime->tm_sec;
	RTC_TimeStructure.RTC_Minutes = newTime->tm_min;
	RTC_TimeStructure.RTC_Hours = newTime->tm_hour;

	/*
	 * set date and time
	 */
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

	/*
	 * unlock RTCAccess
	 */
	RTC_Mutex.Give();
}

/**
 * get the actual time from rtc
 */
void CRTCTime::GetTimeFromRTC(){
	struct std::tm* newTime = 0;

	/*
	 * Lock RTCAccess to
	 */
	RTC_Mutex.Take(portMAX_DELAY);

	/*
	 * get Time and Date from RTC
	 */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	/*
	 * fill std c time struct
	 */
	newTime->tm_sec = RTC_TimeStructure.RTC_Seconds;
	newTime->tm_min = RTC_TimeStructure.RTC_Minutes;
	newTime->tm_hour = RTC_TimeStructure.RTC_Hours;
	newTime->tm_mday = RTC_DateStructure.RTC_Date;
	newTime->tm_mon = RTC_DateStructure.RTC_Month;
	newTime->tm_year = RTC_DateStructure.RTC_Year - 1900;  //counting from 1900

	/*
	 * convert tm to time
	 */
	_Time = std::mktime(newTime);


	/*
	 * unlock RTCAccess
	 */
	RTC_Mutex.Give();
}

