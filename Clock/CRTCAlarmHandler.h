/**
 ******************************************************************************
 * @file        CRTCAlarmHandler.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        16.08.2013
 * @project 	ThunderCryer
 ******************************************************************************
 */

#ifndef CRTCALARMHANDLER_H_
#define CRTCALARMHANDLER_H_

  /**
   * @class CRTCAlarmHander
   *
   * @brief this class implements a thread which handles RTC Alarms
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
   *
   */
class CRTCAlarmHandler {
public:
	CRTCAlarmHandler();
	virtual ~CRTCAlarmHandler();
};

#endif /* CRTCALARMHANDLER_H_ */
