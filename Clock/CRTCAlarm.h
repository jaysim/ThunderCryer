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
  struct {
    bool bSunday:1;
    bool bMonday:1;
    bool bTuesdey:1;
    bool bWednesday:1;
    bool bThursday:1;
    bool bFriday:1;
    bool bSaturday:1;
    bool bArmed:1;
  } __attribute__((packed));
  uint8_t bDays;
}sWeekdaysArm ;

typedef enum{
  IN_ACTIVE,
  SKIP_ALARM,
  W4LIGHT,
  W4ALARM,
  W4SNOOZE
}t_AlarmState;

typedef enum{
    ALARM_A = 0,
    ALARM_B,
    NUM_OF_ALARMS
}t_Alarms;

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
  /*
   * instance counter for correspondence with hardware
   */
  static t_Alarms InstanceCount;

  /*
   * instance indicator
   */
  t_Alarms Instance;

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
   * enable flag for light
   */
  bool bLightEnable;

  /**
   * alarm is snoozing
   */
  bool bSnoozing;

  /**
   * next snooze alarm time
   */
  time_t SnoozeAlarmTime;

  /**
   * snooze counter
   */
  uint8_t u8SnoozeCount;

  /**
   * alarm state for alarm statemachine
   */
  t_AlarmState state;

public:
  CRTCAlarm();
  virtual ~CRTCAlarm();

  /**
   * determine the next time this alarm triggers
   * @param tod Time of the day
   * @return next alarm time
   */
  time_t GetNextAlarm(time_t tod);

  /**
   * go to snooze mode
   */
  void Snooze(void);

  /**
   * stop alarm
   */
  void StopAlarm(void);

  /**
   * skip alarm following alarm
   */
  void SkipAlarm(void);

  /**
   * determine the next time this alarm triggers sound
   * @return next alarm time
   */
  time_t GetAlarmTime(void);

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
   * get Alarm state
   * @return actual alarm state
   */
  t_AlarmState GetAlarmState(void);

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
   * handle an Alarm interrupt
   *
   * @param tod               Time of the Day
   */
  void HandleAlarm(time_t tod);

  /**
   * Setup the alarm
   * @param triggers 			weekday triggers
   * @param alarm				alarm time
   * @param lightMinutes		minutes of light fade in before alarm
   * @param snoozeintervall   minutes between snooze alarms
   * @param light				enable for light alarm
   * @param snooze			enable for snooze
   */
  void SetAlarm(sWeekdaysArm triggers, time_t alarm,
                uint8_t lightMinutes, uint8_t snoozeintervall,
                bool light, bool snooze, time_t tod);


};

#endif /* CRTCALARM_H_ */
