/**
 ******************************************************************************
 * @file        CLightHandler.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        16.08.2013
 * @project 	ThunderCryer
 ******************************************************************************
 */

#ifndef CLIGHTHANDLER_H_
#define CLIGHTHANDLER_H_

#include "ch.hpp"
#include "hal.h"
#include "Notifier.h"
#include "stdint.h"

namespace chibios_rt {
  /**
   * @class CLightHander
   *
   * @brief handles light level from different sources
   *
   * listens to light alarm notification
   *
   */
class CLightHandler : public BaseStaticThread<128>{
private:
	/**
	 * pwm duty cycle for light outputs
	 */
	uint16_t u16LightLevel;

    /**
     * semaphore to regulate light level access
     */
    BinarySemaphore semLightLevelSet;


	/**
	 * light in alarm mode
	 */
	bool bLightAlarm;

	/**
	 * pwm frequency for light pwm
	 */
	const uint16_t u16LightPWMFreq = 1000;

	/**
	 * pwm maximum value
	 */
	const uint16_t u16LightPWMMax = 1000;
protected:
	virtual msg_t main(void);

public:
	CLightHandler();
	virtual ~CLightHandler();
	uint8_t GetLightLevel(void);
	void SetLightLevel(uint8_t level);
};

} /* namespace chibios_rt */

/**
 * @class CLightLevelNotification
 *
 * @brief signal for incomming light alarm
 *
 */
class CLightAlarmNotification : public NotifierMsg<CLightAlarmNotification> {
public:
	uint8_t u8LightMinutes;
};


Notifier<CLightAlarmNotification> notifyLightAlarm;

#endif /* CLIGHTHANDLER_H_ */
