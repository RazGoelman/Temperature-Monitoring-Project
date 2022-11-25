/*
 * Rtc.h

 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"

typedef struct DateTime_
{
	int sec;
	int min;
	int hours;
	int weekDay;
	int day;
	int month;
	int year;
} DateTime;

class _RTC{
private :
	I2C_HandleTypeDef * _hi2c;
	uint8_t _devAddr;
	DateTime * dateTime;
public:

	 _RTC(I2C_HandleTypeDef * hi2c, uint32_t devAddr);


	void rtcStart();

	void rtcStop();

	int rtcIsRunning();

	void rtcGetTime();


	void rtcSetTime(DateTime * _datetime);
};



#endif /* INC_RTC_H_ */
