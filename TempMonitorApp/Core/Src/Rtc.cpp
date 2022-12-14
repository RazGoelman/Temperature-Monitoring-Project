/*
 * Rtc.cpp

 */

#include "Rtc.h"
#include "SystemManager.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "fatfs.h"
#include "time.h"
#include "ff.h"
#define RTC_START_STOP      (1 << 7)
#define RTC_DATE_TIME_SIZE  7
#define DEVICE_ADDR         0xD0


//some variables for FatFs
extern FATFS 	FatFs; 	//Fatfs handle
extern FIL 		fil; 	//File handle
extern FRESULT 	fres; 	//Result after operations
char 			readBuf[100];
extern TCHAR* 	path;


const uint32_t DaysInYear    = 365;
const uint32_t SecondsInMin  = 60;
const uint32_t SecondsInHour = 3600;
const uint32_t SecondsInDay  = 86400;

static const int _daysUntilMonth[] = {
		0,
		31,
		59,
		90,
		120,
		151,
		181,
		212,
		243,
		273,
		304,
		334,
		365
};

_RTC::_RTC(I2C_HandleTypeDef * hi2c, uint32_t devAddr)
{
  _hi2c = hi2c;
  _devAddr = devAddr;
}
void _RTC::rtcStart()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	sec &= ~RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}
void _RTC::rtcStop()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c,_devAddr, 0, 1, &sec, 1, 0xFF);
	sec |= RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}
int _RTC::rtcIsRunning()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	return (sec & RTC_START_STOP) == 0;
}
static int bcdToInt(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}
static uint8_t intToBcd(int value, int minVal, int maxVal)
{
	if (value < minVal || value > maxVal) {
		return 0;
	}

	return ((value / 10) << 4) | (value % 10);
}
void _RTC::rtcGetTime()
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];
	DateTime * mytime = new DateTime;
	if(HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF) == HAL_OK){
	}
	else{
		printf("Read failed\r\n");
	}
	// remove stop bit if set
	buffer[0] 								&= ~RTC_START_STOP;
	mytime->sec 						 	= bcdToInt(buffer[0]);
	mytime->min 							= bcdToInt(buffer[1]);
	mytime->hours 							= bcdToInt(buffer[2]);
	mytime->weekDay 						= buffer[3] & 0x07;
	mytime->day 							= bcdToInt(buffer[4]);
	mytime->month 							= bcdToInt(buffer[5]);
	mytime->year 							= bcdToInt(buffer[6]);
	printf("Date: %02d:%02d:%02d  %02d %02d/%02d/%02d \r\n ",mytime->hours,mytime->min,mytime->sec,mytime->weekDay,mytime->day,mytime->month,mytime->year);
}
void _RTC::rtcSetTime(DateTime * _datetime)
{
	dateTime 								= _datetime;
	uint8_t buffer[RTC_DATE_TIME_SIZE];

	buffer[0] 								= intToBcd(dateTime->sec, 0, 59);
	buffer[1] 								= intToBcd(dateTime->min, 0, 59);
	buffer[2] 								= intToBcd(dateTime->hours, 0, 59);
	buffer[3] 								= dateTime->weekDay < 1 || dateTime->weekDay > 7 ? 0 : dateTime->weekDay;
	buffer[4] 								= intToBcd(dateTime->day, 1, 31);
	buffer[5] 								= intToBcd(dateTime->month, 1, 12);
	buffer[6] 								= intToBcd(dateTime->year, 1, 99);

	if(HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF) == HAL_OK){
		printf("Write to RTC\r\n");
	}
	else{
		printf("Write FAILED\r\n");
	}
}
void _RTC::writeToThresholdFileSD(const char * data)
{
	//Now let's try and write a file "write.txt"
	fres = f_open(&fil, "logger.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres != FR_OK)
	{
		printf("f_open error logger.txt writeToThresholdFileSD (%i)\r\n", fres);
	}
	// write temperature
	strcpy((char*)readBuf, data);
	UINT bytesWrote;
	fres = f_write(&fil, (char*)readBuf,strlen(data), &bytesWrote);

	if(fres != FR_OK) {

		printf("f_write error logger.txt writeToThresholdFileSD (%i)\r\n", fres);
	}
	f_close(&fil);
}

void _RTC::writeToNormalFileSD(const char * data)
{
	fres = f_open(&fil, "logger2.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres != FR_OK)
	{
		printf("f_open logger2.txt error writeToNormalFileSD (%i)\r\n", fres);
	}
	// write temperature
	strcpy((char*)readBuf, data);
	UINT bytesWrote;
	fres = f_write(&fil, (char*)readBuf,strlen(data), &bytesWrote);

	if(fres != FR_OK) {

		printf("f_write logger2.txt error writeToNormalFileSD (%i)\r\n", fres);
	}
	f_close(&fil);
}

void _RTC::readThresholdFileFromSD()
{
	fres = f_open(&fil, "logger.txt", FA_READ | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres != FR_OK)
	{
		printf("f_open error logger.txt readThresholdFileFromSD (%i)\r\n", fres);
	}
	TCHAR* path = f_gets((TCHAR*)readBuf, 100, &fil);
	if (path == 0)
	{
		printf("f_gets error logger.txt readThresholdFileFromSD (%i)\r\n", fres);
	}
	f_close(&fil);
}


void _RTC::readNormalFileFromSD()
{
	fres = f_open(&fil, "logger2.txt", FA_READ | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres != FR_OK)
	{
		printf("f_open error logger2.txt readThresholdFileFromSD (%i)\r\n", fres);
	}
	TCHAR* path = f_gets((TCHAR*)readBuf, 100, &fil);
	if (path == 0)
	{
		printf("f_gets error logger2.txt readThresholdFileFromSD (%i)\r\n", fres);
	}
	f_close(&fil);
}
