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
#include "Cli.h"


//some variables for FatFs
extern FATFS FatFs; 	//Fatfs handle
extern FIL fil; 		//File handle
extern FRESULT fres; //Result after operations
char buf[100];
extern TCHAR* path;

HAL_StatusTypeDef FLASHCORE :: erasePage()
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef flashErase;
	flashErase.TypeErase = FLASH_TYPEERASE_PAGES;
	flashErase.Page = _page;
	flashErase.Banks = _bank;
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

int FLASHCORE :: getCriticalThreshold()
{
	return _thresholds._critical;
}
// setting and save warning level
void FLASHCORE :: setWarningThreshold(int warning)
{
	_thresholds._warningTempThreshold = DEFAULT_TEMP;
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
		printf("Error in write Warning Threshold to page");
	}
	else
	{
		printf("Warning Threshold saved in flash\n");
	}
}
// setting and save critical level
void FLASHCORE :: setCriticalThreshold(int critical)
{
	_thresholds._criticalTempThreshold = DEFAULT_TEMP;
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
		printf("Error in write Critical Threshold to page");
	}
	else
	{
		printf("Critical Threshold saved in flash\n");
	}
}
//get some statistics from the SD card
void FLASHCORE :: SDData(){
	printf("\r\n~  SD card  ~\r\n\r\n");

	HAL_Delay(1000); //a short delay is important to let the SD card settle
	//Open the file
	fres = f_open(&fil, "logger.txt", FA_READ);
	if(fres != FR_OK)
	{
	  printf("File opening logger.txt Error SDData: (%i)\r\n", fres);
	}
	//Let's get some statistics from the SD card
	DWORD free_clusters, free_sectors, total_sectors;

	FATFS* getFreeFs;

	fres = f_getfree("logger.txt", &free_clusters, &getFreeFs);
	if (fres != FR_OK) {
	printf("f_getfree error SDData (%i)\r\n", fres);
	while(1);
	}

	//Formula comes from ChaN's documentation
	total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	free_sectors = free_clusters * getFreeFs->csize;

	printf("SD card status:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

	f_close(&fil);
	printf("Closing File!!!\r\n");
}
//remove file from  SD card
void FLASHCORE::removeFileFromSD()
{
	//Open the file
	fres = f_open(&fil, "logger.txt", FA_READ);
	if(fres != FR_OK)
	{
	  printf("File opening Error removeFileFromSD: (%i)\r\n", fres);
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


}


