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
#include "CLightHandler.h"
#include "ch.hpp"
#include "hal.h"
#include "stm32f4xx.h"

using namespace std;
using namespace chibios_rt;

t_Alarms CRTCAlarm::InstanceCount = ALARM_A;

CRTCAlarm::CRTCAlarm():Instance(InstanceCount){
  InstanceCount = ALARM_B;
}

CRTCAlarm::~CRTCAlarm() {

}

/**
 * determine the next time this alarm triggers
 * @param tod Time of the day
 * @return next alarm time
 */
time_t CRTCAlarm::GetNextAlarm(time_t tod){
  struct tm *sTodTime;
  struct tm *sAlarmTime;
  time_t newAlarm;
  time_t diffLight, diffAlarm;
  int i = 0;
  RTCAlarm rtcAlarm;
  bool alarmChanged = false;

  // check if alarm is valid armed
  // general arm is bit 7, and also one of the bits 0-6 are needed
  if(sAlarmTriggers.bDays > (1<<7)){

    sTodTime = gmtime(&tod);
    sTodTime->tm_hour += 1;	// our time zone

    sAlarmTime = gmtime(&AlarmTime);
    sAlarmTime->tm_hour += 1; // our time zone

    // determine next weekday to trigger
    for (i = 0; i < 7; ++i) {
      sTodTime->tm_wday += i; // go through the weekdays

      if(sTodTime->tm_wday > 6){ // warp around on sunday
        sTodTime->tm_wday = 0;
      }
      // check if alarm should trigger
      if(((1<<sTodTime->tm_wday) & sAlarmTriggers.bDays)!=0){
        break; // exit loop
      }

    }

    // add the days no alarm triggers to date
    // note: if mday is out of range date will be corrected to next month
    sTodTime->tm_mday += i;

    // copy alarm time in full date
    sTodTime->tm_hour = sAlarmTime->tm_hour;
    sTodTime->tm_min = sAlarmTime->tm_min;
    AlarmTime = mktime(sTodTime);
    //build light alarm time, sub according seconds from alarm time
    LightTime = AlarmTime - ((time_t)u8LightMinutes)*60;

    // build differences to tod and check in which phase we are
    diffLight = LightTime - tod;
    diffAlarm = AlarmTime - tod;

    if((diffLight > 0)&&(bLightEnable == true)&&(state != SKIP_ALARM)){
      // light alarm is in future and enabled
      newAlarm = LightTime;
      alarmChanged = true;
      state = W4LIGHT;
    }else{
      // not light alarm or in past or skiped
      if((diffAlarm > 0)&&(state != SKIP_ALARM)){
        //Alarm is in Future
        newAlarm = AlarmTime;
        alarmChanged = true;
        SnoozeAlarmTime = AlarmTime + ((time_t)u8SnoozeIntervall)*60;
        u8SnoozeCount = 0;
        state = W4ALARM;
      }else{
        // wait for alarm in the past to release next alarm
        if((diffAlarm < 0)){
          state = IN_ACTIVE;
        }
      }
    }

    /* Alarm is in Snooze Mode then return snooze time */
    if(state == W4SNOOZE){
      while(SnoozeAlarmTime - tod < 0){
        // as long as snooze is not in future
        // add one intervall
        SnoozeAlarmTime = SnoozeAlarmTime + ((time_t)u8SnoozeIntervall)*60;
        alarmChanged = true;
        u8SnoozeCount++;
      }
    }

    /*
     * do we have to set new alarm in rtc
     */
    if(alarmChanged == true){
      // convert new time to time struct
      sAlarmTime = gmtime(&newAlarm);

      rtcAlarm.tv_datetime = (RTC_ALRMAR_MSK1);
      rtcAlarm.tv_datetime |= ((sAlarmTime->tm_min%10)<<8); // minutes units
      rtcAlarm.tv_datetime |= ((sAlarmTime->tm_min/10)<<12); // minutes tens
      rtcAlarm.tv_datetime |= ((sAlarmTime->tm_hour%10)<<16); // hour units
      rtcAlarm.tv_datetime |= ((sAlarmTime->tm_hour/10)<<20); // hour tens
      rtcAlarm.tv_datetime |= ((sAlarmTime->tm_mday%10)<<24); // mday units
      rtcAlarm.tv_datetime |= ((sAlarmTime->tm_mday/10)<<28); // mday tens

      /*
       * set the alarm in hardware
       */
      rtcSetAlarm(&RTCD1, Instance + 1, &rtcAlarm);

    }

  }else{
    // set alarm to inactive
    state = IN_ACTIVE;
  }

  return newAlarm;
}

/**
 * go to snooze mode
 */
void CRTCAlarm::Snooze(void){
  state = W4SNOOZE;
}

/**
 * stop alarm
 */
void CRTCAlarm::StopAlarm(void){
  state = IN_ACTIVE;
}

/**
 * determine the next time this alarm triggers sound
 * @return next alarm time
 */
time_t CRTCAlarm::GetAlarmTime(void){
  return AlarmTime;
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
 * skip alarm following alarm
 */
void CRTCAlarm::SkipAlarm(void){
  state = SKIP_ALARM;
}


/**
 * handle an Alarm interrupt
 *
 * @param tod               Time of the Day
 */
void CRTCAlarm::HandleAlarm(time_t tod){
  CLightAlarmNotification *light;

  /* handle the alarm states */
  switch(state){
  case IN_ACTIVE:
    /* why there was an interrupt? */
    /* deactivate alarm */
    rtcSetAlarm(&RTCD1, Instance + 1, NULL);
    break;
  case SKIP_ALARM:
    /* alarm skipped then go to inavtive */
    state = IN_ACTIVE;
    break;
  case W4LIGHT:
    /* was waiting for light, then god be good let there be light */
    state = W4ALARM;
    /* bread cast light alarm request */
    light = notifyLightAlarm.alloc();
    if(light != NULL){
      light->u8LightMinutes = u8LightMinutes;
      notifyLightAlarm.broadcast(light);
    }
    break;
  case W4ALARM:
    // TODO: not implementated yet
    break;
  case W4SNOOZE:
    // TODO: not implementated yet
    break;
  default:
    /* should not happen but if get constent by setting inactive */
    state = IN_ACTIVE;
    break;

  }

  /* update Alarms and Hardware */
  GetNextAlarm(tod);
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
