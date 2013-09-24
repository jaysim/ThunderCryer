/**
 ******************************************************************************
 * @file        CRTCHander.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        12.08.2013 21:26:40
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CRTCHANDER_H_
#define CRTCHANDER_H_

#include "ch.hpp"
#include "hal.h"
#include "Notifier.h"
#include "chrtclib.h"
#include "CRTCAlarm.h"

namespace chibios_rt {


typedef enum{
	ALARM_1 = 0,
	ALARM_2,
	ALARM_3,
	NUM_OF_ALARMS
}t_Alarms;


/**
 * @class CActualTime
 *
 * @brief one second broadcast of actual time
 *
 */
class CActualTime : public NotifierMsg<CActualTime> {
public:
    time_t time;
};


  /**
   * @class CRTCHander
   *
   * @brief this class implements a thread which handles RTC Alarms
   *        DCF77 and so on
   *
   * supply 3 alarm options:
   *  - single weekdays the alarm is armed
   *  - alarm time
   *  - light time
   *  - alarm song
   *  - alarm mode
   *  - snooze time
   *  - snooze enable method
   *
   *  thread checks for next armed alarm
   *
   *  check DCF77 input for signal
   *   - build DCF77 time and set RTC accordingly
   *
   */
  class CRTCHander : public BaseStaticThread<1024>{
  private:
	  t_Alarms		activeAlarm;
	  CRTCAlarm		alarms[NUM_OF_ALARMS];
	  CActualTime *tod;
	  CActualTime *alarm;
  protected:
    virtual msg_t main(void);
  public:
    /**
     * Setup the alarm
     * @param index               index of alarm to set
     * @param triggers            Weekday triggers
     * @param alarm               Alarm time
     * @param lightMinutes        Minutes of light fade in before alarm
     * @param snoozeintervall   Minutes between snooze alarms
     * @param light               Enable for light alarm
     * @param snooze          Enable for snooze
     * @param tod                 Time of the day
     */
    void SetAlarm(t_Alarms index, sWeekdaysArm triggers, time_t alarm,
                             uint8_t lightMinutes, uint8_t snoozeintervall,
                             bool light, bool snooze, time_t tod);
    CRTCHander();
    virtual ~CRTCHander();
  };



extern Notifier<CActualTime> notifyActTime;
extern Notifier<CActualTime> notifyActAlarm;




} /* namespace chibios_rt */
#endif /* CRTCHANDER_H_ */
