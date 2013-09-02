/**
 ******************************************************************************
 * @file        CRTCAlarm.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        13.08.2013 08:35:40
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CRTCALARM_H_
#define CRTCALARM_H_

#include <ctime>
#include "stdint.h"

typedef union {
	struct{
		bool bMonday:1;
		bool bTuesdey:1;
		bool bWednesday:1;
		bool bThursday:1;
		bool bFriday:1;
		bool bSaturday:1;
		bool bSunday:1;
		bool bArmed:1;
	}sDays __attribute__((__packed__));
	uint8_t bDays;
}sWeekdaysArm ;


 /**
   * @class CRTCAlarm
   *
   * @brief this class implements the alarms used from CRTCHandler class
   *
   * supply 3 alarm options:
   *  - single weekdays the alarm is armed
   *  - alarm time
   *  - light time
   *  - alarm song  - not yet implemented
   *  - alarm mode
   *  - snooze time
   *  - snooze enable method
   *
   *  supply checks for next armed alarm
   *  supply set of next armed alarm
   *
   */
class CRTCAlarm {
private:
	/**
	 * flags for weekdays on which alarm is armed
	 * the last flag is the general armed flag
	 */
	sWeekdaysArm sAlarmTriggers;

	/**
	 * alarm time of the day (date is next armed weekday or today if not armed)
	 */
	time_t AlarmTime;

	/**
	 * Light alarm start time (behaves as Alarm time)
	 * usualy u8LightMinites before Alarm time
	 */
	time_t LightTime;

	/**
	 * minutes the light starts increasing before alarm
	 */
	uint8_t u8LightMinutes;

	/**
	 * snooze interval in minutes
	 */
	uint8_t u8SnoozeIntervall;

	/**
	 * enable flag for snooze
	 */
	bool bSnoozeEnable;

	/**
	 * enalbe flag for light
	 */
	bool bLightEnable;

public:
	CRTCAlarm();
	virtual ~CRTCAlarm();

	/**
	 * determine the next time this alarm triggers
	 * @param tod Time of the day
	 * @return next alarm time
	 */
	CTime GetNextAlarm(CTime tod);

	/**
	 * determine the next time this alarm triggers sound
	 * @return next alarm time
	 */
	CTime GetAlarmTime(void);

	/**
	 * determine the next time this alarm triggers in snooze mode
	 * @return next alarm time
	 */
	CTime GetNextSnoozeTime(void);

	/**
	 * determine the next time this alarm triggers light
	 * @return next light alarm time
	 */
	CTime GetLightAlarmTime(void);

	/**
	 * get the state of light alarm enable
	 * @return true - light alarm is enabled, false - otherwise
	 */
	bool GetLightEnable(void);

	/**
	 * get the state of snooze alarm enable
	 * @return true - snooze alarm is enabled, false - otherwise
	 */
	bool GetSnoozeEnable(void);

	/**
	 * get the minutes light starts before sound alarm
	 * @return minutes of light fade in before alarm
	 */
	uint8_t GetLightMinutes(void);

	/**
	 * arm/disarm the Alarm
	 * @param arm - arm state of the alarm
	 */
	void SetAlarmArmed(bool arm);

	/**
	 * Setup the alarm
	 * @param triggers 			weekday triggers
	 * @param alarm				alarm time
	 * @param lightMinutes		minutes of light fade in before alarm
	 * @param snoozeintervall   minutes between snooze alarms
	 * @param light				enable for light alarm
	 * @param snooze			enable for snooze
	 */
	void SetAlarm(sWeekdaysArm triggers, CTime alarm, uint8_t lightMinutes, uint8_t snoozeintervall, bool light, bool snooze);


};

#endif /* CRTCALARM_H_ */
