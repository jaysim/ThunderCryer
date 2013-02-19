/**
 ******************************************************************************
 * @file      	CTime.h
 * @author    	Tecnologic86
 * @version   	V0.0.0
 * @date      	03.07.2012 12:39:40
 * @project	ThunderCryer
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CTIME_H_
#define CTIME_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <ctime>
/* namespace -----------------------------------------------------------------*/
namespace Time {

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void CTimeUnitTest();
/* Class definitions -------------------------------------------------------- */
/*
 *
 */
class CTime {

protected:

	std::time_t _Time;


public:
	CTime();
	CTime(CTime const& time);
	CTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year);
	virtual ~CTime();
	const CTime& operator= (const CTime& rhs);
	unsigned int getTimeString(char* buffer);
	unsigned int getDateString(char* buffer);
	unsigned int getWeekNumberString(char* buffer);

	friend CTime operator- (const CTime& left, const CTime& right);
	friend CTime operator+ (const CTime& left, const CTime& right);
	friend CTime operator- (const CTime& left, int64_t seconds);
	friend CTime operator+ (const CTime& left, int64_t seconds);
	friend CTime operator- (int64_t seconds, const CTime& right);
	friend CTime operator+ (int64_t seconds, const CTime& right);
	friend bool operator> (const CTime& left, const CTime& right);
	friend bool operator< (const CTime& left, const CTime& right);
	friend bool operator> (const CTime& left, int64_t seconds);
	friend bool operator< (const CTime& left, int64_t seconds);
	friend bool operator> (int64_t seconds, const CTime& right);
	friend bool operator< (int64_t seconds, const CTime& right);
};

/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
CTime operator+ (const CTime& left, const CTime& right);

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
CTime operator- (const CTime& left, const CTime& right);

/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
CTime operator+ (const CTime& left,int64_t seconds);

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
CTime operator- (const CTime& left, int64_t seconds);


/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
CTime operator+ (int64_t seconds, const CTime& right);

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
CTime operator- (int64_t seconds, const CTime& right);

/**
 * global higher than operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result true when left is higher than right
 */
bool operator> (const CTime& left, const CTime& right);

/**
 * global lower than operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result true when left is lower then right
 */
bool operator< (const CTime& left, const CTime& right);

/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
bool operator> (const CTime& left,int64_t seconds);

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
bool operator< (const CTime& left, int64_t seconds);


/**
 * global add operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left + right
 */
bool operator> (int64_t seconds, const CTime& right);

/**
 * global sub operator operator
 *
 * Ref: Der-C++-Programmierer S. 321
 * @param left operand left of sign
 * @param right operand right of sign
 * @return result of addition left - right
 */
bool operator< (int64_t seconds, const CTime& right);


} /* namespace Time */
#endif /* CTIME_H_ */
