/**
 ******************************************************************************
 * @file      	CRTCTime.h
 * @author    	Tecnologic86
 * @version   	V0.0.0
 * @date      	11.07.2012 08:39:40
 * @project	ThunderCryer
 ******************************************************************************
 */

#ifndef CRTCTIME_H_
#define CRTCTIME_H_

#include "CTime.h"
#include "CMutex.h"
#include "stm32f4xx_rtc.h"

class CRTCTime: public Time::CTime {
private:
	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	CMutex 			RTC_Mutex;

public:
	void Init();
	void SetTimeInRTC();
	void GetTimeFromRTC();
	CRTCTime();
	virtual ~CRTCTime();
};

#endif /* CRTCTIME_H_ */
