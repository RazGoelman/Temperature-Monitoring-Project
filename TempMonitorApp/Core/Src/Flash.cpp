/*
 * Flash.cpp

 */


#include "Flash.h"
#include <stdio.h>
#include <string.h>


HAL_StatusTypeDef Flash :: erasePage()
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

HAL_StatusTypeDef Flash :: writeToPage(void* data, int dataSize)
{
	HAL_StatusTypeDef status;
	HAL_FLASH_Unlock();
	int index = 0;
	while (index < dataSize)
	{
		uint64_t Data =*(uint64_t*)(data+index); // add template
		status = HAL_FLASH_Program(_typeProgram, _pageAddr+index, Data);
		if(status != HAL_OK)
		{
			return status;
		}
		index += sizeof(uint64_t);
	}
	HAL_FLASH_Lock();
	//printf("%d\r\n", status);
	return status;
}

int Flash :: getWarning()
{
	return _thresholds._warning;
}

int Flash :: getCritical()
{
	return _thresholds._critical;
}

void Flash :: setWarning(int warning)
{
	_thresholds._warningUsed = DATA_IN_USED;
	_thresholds._warning = warning;
	HAL_StatusTypeDef status;
	status = erasePage();
	if(status != HAL_OK)
	{
		printf("error in erase page in line %d in file %s\r\n", __LINE__, __FILE__);
	}
	status = writeToPage( &_thresholds, (sizeof(THRESHOLDS)));
	if(status != HAL_OK)
	{
		printf("error in write to page in line %d in file %s\r\n", __LINE__, __FILE__);
	}
	else
	{
		printf("saved in flash in line %d in file %s \r\n", __LINE__, __FILE__);
	}
}

void Flash :: setCritical(int critical)
{
	_thresholds._criticalUsed = DATA_IN_USED;
	_thresholds._critical = critical;
	HAL_StatusTypeDef status;
	status = erasePage();
	if(status != HAL_OK)
	{
		printf("error in erase page in line %d in file %s\r\n", __LINE__, __FILE__);
	}
	status = writeToPage( &_thresholds, (sizeof(THRESHOLDS)));
	if(status != HAL_OK)
	{
		printf("error in write to page in line %d in file %s\r\n", __LINE__, __FILE__);
	}
	else
	{
		printf("saved in flash in line %d in file %s \r\n", __LINE__, __FILE__);
	}
}

void Flash :: printThresHolds()
{
	THRESHOLDS* data = (THRESHOLDS *)(_pageAddr);
	memcpy(&_thresholds, data, sizeof(THRESHOLDS));
	_thresholds._criticalUsed == DATA_IN_USED? printf("critical = %d \r\n", _thresholds._critical):
											  printf("Please insert critical temp\r\n");

	_thresholds._warningUsed == DATA_IN_USED? printf("warning = %d \r\n", _thresholds._warning):
			  	  	  	  	  	  	  	  	 printf("Please insert warning temp\r\n");
}
