/**
 ******************************************************************************
 * @file      	CTime_test.cpp
 * @author    	Tecnologic86
 * @version   	V0.0.0
 * @date      	03.07.2012 12:39:40
 * @project	ThunderCryer
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "CTime.h"

/* namespace -----------------------------------------------------------------*/
namespace Time {

/**
 * Unit Test for CTime class
 */
void CTimeUnitTest(){
	CTime today(11,28,14,5,7,2012);  // 14:28:11 5.7.12 Thu Week 27
	CTime nextday(today);
	CTime yesterday,diff1,diff2,min30;
	char buffer[40];

	nextday = today + (3600 * 24); // add 1 day in seconds
	yesterday = today - (3600 * 24); // sub 1 day

	diff1 = today - yesterday;
	diff2 = nextday - 869000;
	min30 = 30 * 60 + today;

	today.getTimeString(buffer);

	diff1.getDateString(buffer);

	yesterday.getWeekNumberString(buffer);
}

} /* namespace Time */
