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
#include "stm32f4xx.h"
#include "main.h"

namespace chibios_rt {

  Notifier<CActualTime> notifyActTime;
  Notifier<CActualTime> notifyActAlarm;
  BinarySemaphore OneSecTick(false);
  BinarySemaphore AlarmTick[NUM_OF_ALARMS] =
                  {BinarySemaphore(false), BinarySemaphore(false)};

  void extcbAlarm(EXTDriver *extp, expchannel_t channel);
  void extcbOneSec(EXTDriver *extp, expchannel_t channel);



  static const EXTConfig extcfg = {
                                   {
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_FALLING_EDGE | EXT_MODE_GPIOC, extcbWifi},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART, extcbAlarm},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_DISABLED, NULL},
                                    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART, extcbOneSec}
                                   }
  };


  /* Triggered when RTC has an event*/
  void extcbAlarm(EXTDriver *extp, expchannel_t channel) {
    (void)extp;
    (void)channel;

    /*
     * Alarm A Interrupt
     */
    if((RTC->ISR & RTC_ISR_ALRAF) != 0){
      RTC->ISR &= ~RTC_ISR_ALRAF; // clear Flag
      /*
       * Signal handler Alarm A was there
       */
      AlarmTick[ALARM_A].signal();
    }

    /*
     * Alarm B Interrupt
     */
    if((RTC->ISR & RTC_ISR_ALRBF) != 0){
      RTC->ISR &= ~RTC_ISR_ALRBF; // clear Flag
      /*
       * Signal handler Alarm B was there
       */
      AlarmTick[ALARM_B].signal();
    }

  }


  /* Triggered when RTC has an event*/
  void extcbOneSec(EXTDriver *extp, expchannel_t channel) {
    (void)extp;
    (void)channel;

    /*
     * 1 sec Wakeup interupt
     */
    if((RTC->ISR & RTC_ISR_WUTF) != 0){
      RTC->ISR &= ~RTC_ISR_WUTF; // clear Flag
      /*
       * Signal Handler to send new Time broadcast
       */
      System::lockFromIsr();
      OneSecTick.signalI();
      System::unlockFromIsr();
    }
  }




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
    static time_t todAlarmDiff;
    static time_t minTodAlarmDiff;
    static time_t nextAlarmTime;
    static t_Alarms nextAlarm;
    static RTCWakeup wakeupspec;
    Listener<CDCFNewTimeArrived,5> listenerDCF(&notifyDCFTime);


    setName("RTC_Handler");

    /* init rtc */

    /*
     * Activates the EXT driver 1.
     * for RTC Interrupt
     */
    extStart(&EXTD1, &extcfg);
    extChannelDisable(&EXTD1,GPIOC_WIFI_IRQ);

    /* set wakeup */
    /* for time update every second */
    wakeupspec.wakeup = ((uint32_t)3) << 16; /* select RTC clk/2 clock source */
    wakeupspec.wakeup |= (32768/2)-1; /* set counter value to 65535 Period will be 1 second. */
    rtcSetPeriodicWakeup_v2(&RTCD1, &wakeupspec);



    /* set initial time of day */

    while(true){
      /*
       * one second wake up has come
       */
      OneSecTick.wait();

      // broadcast new tod
      // get new element on mailbox
      tod = notifyActTime.alloc();
      if(tod != NULL){
        tod->time = rtcGetTimeUnixSec(&RTCD1);
        notifyActTime.broadcast(tod);
      }

      // check for new dcf77 time
      // if new time available
      // set it
      if(listenerDCF.available()){
        CDCFNewTimeArrived* dcf = listenerDCF.get();
        rtcSetTimeUnixSec(&RTCD1, dcf->newTime);
        listenerDCF.release(dcf);
      }

      // check next alarm
      // and set it
      // next alarm notification
      todAlarmDiff = tod->time; // init diff value with very high value
      minTodAlarmDiff = tod->time;

      for (int i = 0; i < NUM_OF_ALARMS; ++i) {
        nextAlarmTime = alarms[i].GetNextAlarm(tod->time);
        todAlarmDiff = nextAlarmTime - tod->time;
        if(minTodAlarmDiff < todAlarmDiff){
          minTodAlarmDiff = todAlarmDiff;  // save next alarm
          nextAlarm = (t_Alarms)i;
        }
      }

      // broadcast new alarm
      if(activeAlarm != nextAlarm){
        alarm = notifyActAlarm.alloc();
        if(alarm != NULL){
          alarm->time = alarms[nextAlarm].GetAlarmTime();
          notifyActAlarm.broadcast(alarm);
        }
      }

      /*
       * check for alarm interrupts
       * and let the objects handle them
       */
      for(int i; i < NUM_OF_ALARMS; i++){
        if(AlarmTick[i].waitTimeout(TIME_IMMEDIATE) == RDY_OK){
          alarms[i].HandleAlarm(tod->time);
        }
      }


    }
    return 0;
  }


} /* namespace chibios_rt */
