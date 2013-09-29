/**
 ******************************************************************************
 * @file        CRTCHander.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        12.08.2013 21:26:40
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CRTCHandler.h"
#include "CDCF77.h"
#include "ch.hpp"
#include "hal.h"
#include "time.h"
#include "chrtclib.h"

namespace chibios_rt {

  Notifier<CActualTime> notifyActTime;
  Notifier<CActualTime> notifyActAlarm;



  CRTCHander::CRTCHander() {
    // TODO Auto-generated constructor stub

  }

  CRTCHander::~CRTCHander() {
    // TODO Auto-generated destructor stub
  }


  /**
   * Setup the alarm
   * @param index				index of alarm to set
   * @param triggers 			Weekday triggers
   * @param alarm				Alarm time
   * @param lightMinutes		Minutes of light fade in before alarm
   * @param snoozeintervall   Minutes between snooze alarms
   * @param light				Enable for light alarm
   * @param snooze			Enable for snooze
   * @param tod 				Time of the day
   */
  void CRTCHander::SetAlarm(t_Alarms index, sWeekdaysArm triggers, time_t alarm,
                           uint8_t lightMinutes, uint8_t snoozeintervall,
                           bool light, bool snooze, time_t tod){

	  alarms[index].SetAlarm(triggers, alarm, lightMinutes, snoozeintervall,
			  	  	  	  	  light, snooze, tod);

  }


  msg_t CRTCHander::main(void){
	  static systime_t tCycleStart;
	  static time_t todAlarmDiff;
	  static time_t minTodAlarmDiff;
	  static t_Alarms nextAlarm;
	  Listener<CDCFNewTimeArrived,5> listenerDCF(&notifyDCFTime);


	  setName("RTC_Handler");

	  /* init rtc */
	  /* set initial time of day */

	  while(true){
		  tCycleStart = System::getTime();

		  // check for new dcf77 time
		  	  // if new time available
		  	  // set it
		  if(listenerDCF.available()){
		    CDCFNewTimeArrived* dcf = listenerDCF.get();
		    rtcSetTimeUnixSec(&RTCD1, dcf->newTime);
		    listenerDCF.release(dcf);
		  }


	      // broadcast new tod
	      // get new element on mailbox
	      tod = notifyActTime.alloc();
	      if(tod != NULL){
	        tod->time = rtcGetTimeUnixSec(&RTCD1);
	        notifyActTime.broadcast(tod);
	      }

	  	  // check next alarm
	  	  	  // and set it
	  	  	  // next alarm notification
	      todAlarmDiff = tod->time; // init diff value with very high value
	      minTodAlarmDiff = tod->time;

		  for (int i = 0; i < NUM_OF_ALARMS; ++i) {
			  todAlarmDiff = alarms[i].GetNextAlarm(tod->time) - tod->time;
			  if(minTodAlarmDiff < todAlarmDiff){
				  minTodAlarmDiff = todAlarmDiff;  // save next alarm
				  nextAlarm = (t_Alarms)i;
			  }
		  }

		  if(activeAlarm != nextAlarm){
			  // broadcast new alarm
			  alarm = notifyActAlarm.alloc();
			  if(alarm != NULL){
			    alarm->time = alarms[nextAlarm].GetAlarmTime();
			    notifyActAlarm.broadcast(alarm);
			  }
		  }


	  	  //check for alarm
	  	  	  //check for light enabled
	  	  	  	  // init light alarm notification
	  	  	  	  // set next alarm to alarm time
	  	  	  	  // send notification Lightalarm to light task
		  	  //else
		  	   	  // search and set next armed alarm
		  	   	  // send alarm notification



		  /*
		   * cycle with 1 sec period
		   */
		  sleepUntil(tCycleStart + MS2ST(1000));
	  }




	  return 0;
  }


} /* namespace chibios_rt */
