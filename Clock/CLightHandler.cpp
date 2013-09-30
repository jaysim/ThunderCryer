/**
 ******************************************************************************
 * @file        CLightHandler.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        16.08.2013
 * @project 	ThunderCryer
 ******************************************************************************
 */

#include "CLightHandler.h"


namespace chibios_rt {



CLightHandler::CLightHandler():u16LightLevel(0),semLightLevelSet(false), bLightAlarm(false){
}

CLightHandler::~CLightHandler() {
}

uint8_t CLightHandler::GetLightLevel(void){
	return (u16LightLevel*100)/u16LightPWMMax;
}

/**
 * set the light level from external thread
 * @param level
 */
void CLightHandler::SetLightLevel(uint8_t level){
	/* deactivate light alarm */
	bLightAlarm = false;

	/* wait for access */
	semLightLevelSet.wait();

	u16LightLevel = PWM_PERCENTAGE_TO_WIDTH(&PWMD5, level);

	pwmEnableChannel(&PWMD5,0,u16LightLevel);

	/* release access */
	semLightLevelSet.signal();

}


/**
 * thread function
 * @return nothing useful
 */
msg_t CLightHandler::main(void){
	const uint16_t STEPS = 100;
	static systime_t interval = 0;
	static uint16_t pwmStep = 0;

	/* config listener to alarm notification */
	CLightAlarmNotification *alarm;
	Listener<CLightAlarmNotification,5> listenerAlarm(&notifyLightAlarm);

	/* pwm configuration */
	static PWMConfig pwmcfg = {
			u16LightPWMFreq, /* 10kHz PWM clock frequency.   */
			u16LightPWMMax,  /* Initial PWM period 1S.       */
			NULL,
			{
					{PWM_OUTPUT_ACTIVE_HIGH, NULL},
					{PWM_OUTPUT_ACTIVE_HIGH, NULL},
					{PWM_OUTPUT_DISABLED, NULL},
					{PWM_OUTPUT_DISABLED, NULL}
			},
			0,
			0
	};

	/*
	 * start pwm with 0 level
	 */
	pwmStart(&PWMD5, &pwmcfg);
	pwmEnableChannel(&PWMD5,0,u16LightLevel);


	/* Task loop */
	while(true){
		/* check for the light alarm */
		alarm = listenerAlarm.get();
		/* build intervall in ms, be carefull with numbers its uint16 */
		interval = ((uint16_t)alarm->u8LightMinutes)*(60000/STEPS);
		/* the pwm step needs to be greater than 0 to do just anything */
		pwmStep = ((u16LightPWMMax - u16LightLevel) / STEPS) +1;

		/* set flag that light alarm is avtive */
		bLightAlarm = true;

		/*
		 * step though the sunrise :)
		 */
		for( ; u16LightLevel < u16LightPWMMax; u16LightLevel += pwmStep){
			/* some one set the light to a fixed level */
			if(bLightAlarm == false){
				break;
			}
			/* wait for access */
			semLightLevelSet.wait();

			if(u16LightLevel > u16LightPWMMax){
				u16LightLevel = u16LightPWMMax;
			}
			pwmEnableChannel(&PWMD5,0,u16LightLevel);

			/* release access */
			semLightLevelSet.signal();

			sleep(interval);
		}
		/* reset flag that light alarm is avtive */
		bLightAlarm = false;

	}

	return 0;
}



} /* namespace chibios_rt */
/* EOF */
