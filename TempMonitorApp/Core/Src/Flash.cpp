/*
 * Flash.cpp

 */


#include "Flash.h"
#include <stdio.h>
#include <string.h>
#include "fatfs.h"
#include "time.h"
#include "main.h"
#include "ff.h"


//some variables for FatFs
extern FATFS FatFs; 	//Fatfs handle
extern FIL fil; 		//File handle
extern FRESULT fres; //Result after operations
//BYTE readBuf[30];
char buf[100];

extern TCHAR* path;
extern UART_HandleTypeDef huart2;


void Send_Uart (char *string)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)string, strlen (string), HAL_MAX_DELAY);
}

HAL_StatusTypeDef FLASHCORE :: erasePage()
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef flashErase;
	flashErase.TypeErase = FLASH_TYPEERASE_PAGES;
	flashErase.Banks = _bank;
	flashErase.Page = _pageAddr;
	flashErase.NbPages = _nbPages;

	uint32_t pageError;
	HAL_FLASHEx_Erase(&flashErase, &pageError);
	if(pageError == 0xFFFFFFFF)
	{
		HAL_FLASH_Lock();
		return HAL_OK;
	}
	else
	{
		HAL_FLASH_Lock();
		return HAL_ERROR;
	}

}

HAL_StatusTypeDef FLASHCORE :: writeToPage(void* data, int dataSize)
{
	HAL_StatusTypeDef status;
	HAL_FLASH_Unlock();
	int index = 0;
	while (index < dataSize)
	{
		uint64_t Data =*(uint64_t*)((uint8_t*)data + index);
		status = HAL_FLASH_Program(_typeProgram, _pageAddr + index, Data);
		if(status != HAL_OK)
		{
			return status;
		}
		index += sizeof(uint64_t);
	}
	HAL_FLASH_Lock();
	printf("%d\r\n", status);
	return status;
}
//get warning threshold
int FLASHCORE :: getWarningThreshold()
{
	return _thresholds._warning;
}
//get critical threshold
int FLASHCORE :: getCriticalThreshold()
{
	return _thresholds._critical;
}
// setting and save warning level
void FLASHCORE :: setWarningThreshold(int warning)
{
	_thresholds._warningTempThreshold = DATA_WAITING;
	_thresholds._warning = warning;
	HAL_StatusTypeDef status;
	status = erasePage();
	if(status != HAL_OK)
	{
		printf("Error in erase page");
	}
	status = writeToPage( &_thresholds, (sizeof(THRESHOLDS)));
	if(status != HAL_OK)
	{
		printf("Error in write to page");
	}
	else
	{
		printf("Warning event saved in flash");
	}
}
// setting and save critical level
void FLASHCORE :: setCriticalThreshold(int critical)
{
	_thresholds._criticalTempThreshold = DATA_WAITING;
	_thresholds._critical = critical;
	HAL_StatusTypeDef status;
	status = erasePage();
	if(status != HAL_OK)
	{
		printf("Error in erase page");
	}
	status = writeToPage( &_thresholds, (sizeof(THRESHOLDS)));
	if(status != HAL_OK)
	{
		printf("Error in write to page");
	}
	else
	{
		printf("Critical event saved in flash");
	}
}
// print warning / critical data
void FLASHCORE :: printThresHoldsTemperature()
{
        THRESHOLDS* data = (THRESHOLDS *)(_pageAddr);
        memcpy(&_thresholds, data, sizeof(THRESHOLDS));
        if (_thresholds._criticalTempThreshold == DATA_WAITING){
        	printf("Please insert critical temperature\r\n");
        }
        else{
        	printf("critical = %d \r\n", _thresholds._critical);
        }
        if(_thresholds._warningTempThreshold == DATA_WAITING){
        	printf("Please insert warning temperature\r\n");
        }
        else{
        	printf("warning = %d \r\n", _thresholds._warning);
        }


}
//get some statistics from the SD card
void FLASHCORE :: SDDATA(){
	printf("\r\n~  SD card  ~\r\n\r\n");

	HAL_Delay(1000); //a short delay is important to let the SD card settle

	//Open the file system
	fres = f_mount(&FatFs, "", 1); //1=mount now
	if (fres != FR_OK) {
	printf("f_mount error (%i)\r\n", fres);
	while(1);
	}

	//Let's get some statistics from the SD card
	DWORD free_clusters, free_sectors, total_sectors;

	FATFS* getFreeFs;

	fres = f_getfree("", &free_clusters, &getFreeFs);
	if (fres != FR_OK) {
	printf("f_getfree error (%i)\r\n", fres);
	while(1);
	}

	//Formula comes from ChaN's documentation
	total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	free_sectors = free_clusters * getFreeFs->csize;

	printf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

}

DWORD FLASHCORE::TIMERTC(){

	time_t t;
	    struct tm *stm;


	    t = time(0);
	    stm = localtime(&t);
	    return (DWORD)(stm->tm_year - 80) << 25 |
	           (DWORD)(stm->tm_mon + 1) << 21 |
	           (DWORD)stm->tm_mday << 16 |
	           (DWORD)stm->tm_hour << 11 |
	           (DWORD)stm->tm_min << 5 |
	           (DWORD)stm->tm_sec >> 1;

}

void FLASHCORE::RemoveFile()
{
	do{
	//Open the file system
		fres = f_mount(&FatFs, "", 1); //1=mount now
		if (fres != FR_OK) {
			printf("No SD Card found (%i)\r\n", fres);
			break;
		}
		printf("SD Card Mounted Successfully!!!\r\n");

		//Open the file
		fres = f_open(&fil, "logger.txt", FA_READ);
		if(fres != FR_OK)
		{
		  printf("File opening Error : (%i)\r\n", fres);
		  break;
		}
		//read the data
		f_gets(buf, sizeof(buf), &fil);
		printf("Read Data : %s\r\n", buf);
		//close your file
		f_close(&fil);
		printf("Closing File!!!\r\n");
#if 0
		fres = f_unlink("logger.txt");
		if (fres != FR_OK){
			printf("Cannot able to delete the file\n");
		}
		printf (buf, "*%s* has been removed successfully\n", fres);
#endif
	}while(false);
	//We're done, so de-mount the drive
	  f_mount(NULL, "", 0);
	  printf("SD Card Unmounted Successfully!!!\r\n");

}
