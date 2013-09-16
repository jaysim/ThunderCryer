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

namespace chibios_rt {

  CRTCHander::CRTCHander() {
    // TODO Auto-generated constructor stub

  }

  CRTCHander::~CRTCHander() {
    // TODO Auto-generated destructor stub
  }


  msg_t CRTCHander::main(void){
	  static systime_t tCycleStart;
	  static Listener<CDCFNewTimeArrived,5> listenerDCF(&notifyDCFTime);


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
		  }


		  	  // check next alarm
		  	  // and set it
		  	  // Time of day notification
		  	  // next alarm notification



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
