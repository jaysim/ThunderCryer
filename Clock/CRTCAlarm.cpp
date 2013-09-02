/**
 ******************************************************************************
 * @file        CRTCAlarm.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        13.08.2013 08:35:40
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CRTCAlarm.h"

using namespace std;

CRTCAlarm::CRTCAlarm() {
	// TODO Auto-generated constructor stub

}

CRTCAlarm::~CRTCAlarm() {
	// TODO Auto-generated destructor stub
}

/**
 * determine the next time this alarm triggers
 * @param tod Time of the day
 * @return next alarm time
 */
time_t CRTCAlarm::GetNextAlarm(time_t tod){
	struct tm *pTime;
	int i;

	pTime = gmtime(&tod);
	pTime->tm_hour += 1;	// our time zone
	pTime->tm_wday += 1;    // monday is 0

	for (i = 0; i < 7; ++i) {
		pTime->tm_wday += i; // go through the weekdays

		if(pTime->tm_wday > 6){ // warp around on sunday
			pTime->tm_wday = 0;
		}
		// check if alarm should trigger
		if(((1<<pTime->tm_wday) & sAlarmTriggers.bDays)!=0){
			break; // exit loop
		}

	}

	// add the days no alarm triggers to date
	// note: if mday is out of range date will be corrected to next month
	pTime->tm_mday += i;

	// make unix time
	AlarmTime = mktime(pTime);

	// build lightalarm time
	pTime->tm_min += u8LightMinutes;
	if(pTime->tm_min > 59){
		// if we wrap the minutes, we add an hour
		pTime->tm_hour += (pTime->tm_min / 60);
		pTime->tm_min %= 60;
	}

	LightTime = mktime(pTime);

	return AlarmTime;
}

/**
 * determine the next time this alarm triggers sound
 * @return next alarm time
 */
time_t CRTCAlarm::GetAlarmTime(void){
	return AlarmTime;
}

/**
 * determine the next time this alarm triggers in snooze mode
 * @return next alarm time
 */
time_t CRTCAlarm::GetNextSnoozeTime(void){
	struct tm *pTime;

	pTime = gmtime(&AlarmTime);
	pTime->tm_hour += 1;	// our time zone

	// build snooze time
	pTime->tm_min += u8SnoozeIntervall;
	if(pTime->tm_min > 59){
		// if we wrap the minutes, we add an hour
		pTime->tm_hour += (pTime->tm_min / 60);
		pTime->tm_min %= 60;
	}

	return mktime(pTime);
}

/**
 * determine the next time this alarm triggers light
 * @return next light alarm time
 */
time_t CRTCAlarm::GetLightAlarmTime(void){
	return LightTime;
}

/**
 * get the state of light alarm enable
 * @return true - light alarm is enabled, false - otherwise
 */
bool CRTCAlarm::GetLightEnable(void){
	return bLightEnable;
}

/**
 * get the state of snooze alarm enable
 * @return true - snooze alarm is enabled, false - otherwise
 */
bool CRTCAlarm::GetSnoozeEnable(void){
	return bSnoozeEnable;
}

/**
 * get the minutes light starts before sound alarm
 * @return minutes of light fade in before alarm
 */
uint8_t CRTCAlarm::GetLightMinutes(void){
	return u8LightMinutes;
}

/**
 * arm/disarm the Alarm
 * @param arm - arm state of the alarm
 */
void CRTCAlarm::SetAlarmArmed(bool arm){
	sAlarmTriggers.bArmed = arm;
}

/**
 * Setup the alarm
 * @param triggers 			Weekday triggers
 * @param alarm				Alarm time
 * @param lightMinutes		Minutes of light fade in before alarm
 * @param snoozeintervall   Minutes between snooze alarms
 * @param light				Enable for light alarm
 * @param snooze			Enable for snooze
 * @param tod 				Time of the day
 */
void CRTCAlarm::SetAlarm(sWeekdaysArm triggers, time_t alarm,
                         uint8_t lightMinutes, uint8_t snoozeintervall,
                         bool light, bool snooze, time_t tod){
	sAlarmTriggers = triggers;
	AlarmTime = alarm;
	u8SnoozeIntervall = snoozeintervall;
	bSnoozeEnable = snooze;
	bLightEnable = light;
	u8LightMinutes = lightMinutes;

	// build all alarm times accordingly to tod
	GetNextAlarm(tod);

}
