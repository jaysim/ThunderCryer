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

namespace chibios_rt {

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

  protected:
    virtual msg_t main(void);
  public:
    CRTCHander();
    virtual ~CRTCHander();
  };

/**
 * @class CActualTime
 *
 * @brief one second broadcast of actual time
 *
 */
class CActualTime : public NotifierMsg<CActualTime> {
public:
    time_t tod;
};


extern Notifier<CActualTime> notifyActTime;



} /* namespace chibios_rt */
#endif /* CRTCHANDER_H_ */
