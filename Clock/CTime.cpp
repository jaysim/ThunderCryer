/**
 ******************************************************************************
 * @file      	CTime.cpp
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
 * @brief baseline constructor
 */
CTime::CTime():_Time(0) {
}

/**
 * @brief copy constructor
 * @param time will be copied
 */
CTime::CTime(CTime const& time):_Time(time._Time){
}

/**
 * @brief constructs time by time parameters
 * @param sec seconds after the minute 0-61*
 * @param min minutes after the hour 0-59
 * @param hour hours since midnight 0-23
 * @param day  day of the month 1-31
 * @param month  months since January 0-11
 * @param year  year in yyyy
 *
 */
CTime::CTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year):_Time(0){
	struct std::tm* newTime = 0;

	newTime->tm_sec = sec;
	newTime->tm_min = min;
	newTime->tm_hour = hour;
	newTime->tm_mday = day;
	newTime->tm_mon = month;
	newTime->tm_year = year;

	_Time = std::mktime(newTime);
}

/**
 * @brief standard destructor
 */
CTime::~CTime() {
	// nothing dynamically allocated
}


/**
 * assign operator
 * @param rhs object to assign to this instance
 * @return rhs reference on result
 */
const CTime& CTime::operator= (const CTime& rhs){
	_Time = rhs._Time;
	return *this;
}

/**
 * format time data to hh:mm:ss
 * @return time as string
 */
/**
 * format time data to hh:mm:ss
 * @param buffer bigger than 9 chars
 * @return chars inserted in buffer, 0 if failed
 */
unsigned int CTime::getTimeString(char* buffer){
	return std::strftime(buffer,sizeof(buffer),"%X",std::gmtime(&_Time));
}

/**
 * format date to Thu 21 Aug 2012
 * @param buffer bigger than 16 chars
 * @return chars inserted in buffer, 0 if failed
 */
unsigned int CTime::getDateString(char* buffer){
	return std::strftime(buffer,sizeof(buffer),"%a %d &b %Y",std::gmtime(&_Time));
}

/**
 * format weeknumber to "Week 51"
 * @param buffer bigger than 8 chars
 * @return chars inserted in buffer, 0 if failed
 */
unsigned int CTime::getWeekNumberString(char* buffer){
	return std::strftime(buffer,sizeof(buffer),"Week: %W",std::gmtime(&_Time));
}

/*																						 *
 * ---------------------------------------Operators--------------------------------------*
 * 																						 *
 */

/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
CTime operator+ (const CTime& left, const CTime& right){
	CTime result;

	result._Time = left._Time + right._Time;

	return result;
}

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
CTime operator- (const CTime& left, const CTime& right){
	CTime result;

	result._Time = left._Time - right._Time;

	return result;
}

/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
CTime operator+ (const CTime& left, int64_t seconds){
	CTime result;

	result._Time = left._Time + static_cast<time_t>(seconds);

	return result;
}

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
CTime operator- (const CTime& left, int64_t seconds){
	CTime result;

	result._Time = left._Time - static_cast<time_t>(seconds);

	return result;
}
/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
CTime operator+ (int64_t seconds, const CTime& right){
	CTime result;

	result._Time = static_cast<time_t>(seconds) + right._Time;

	return result;
}

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
CTime operator- (int64_t seconds, const CTime& right){
	CTime result;

	result._Time = static_cast<time_t>(seconds) - right._Time;

	return result;
}

/**
 * global higher than operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result true when left is higher than right
 */
bool operator> (const CTime& left, const CTime& right){
	return left._Time > right._Time;
}

/**
 * global lower than operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result true when left is lower then right
 */
bool operator< (const CTime& left, const CTime& right){
	return left._Time < right._Time;
}

/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
bool operator> (const CTime& left,int64_t seconds){
	return left._Time > static_cast<time_t>(seconds);
}

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
bool operator< (const CTime& left, int64_t seconds){
	return left._Time < static_cast<time_t>(seconds);
}


/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
bool operator> (int64_t seconds, const CTime& right){
	return static_cast<time_t>(seconds) > right._Time;
}

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
bool operator< (int64_t seconds, const CTime& right){
	return static_cast<time_t>(seconds) > right._Time;
}



} /* namespace Time */
