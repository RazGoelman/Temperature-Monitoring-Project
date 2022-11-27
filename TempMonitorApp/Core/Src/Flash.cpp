/*
 * Flash.cpp

 */


#include "Flash.h"
#include <stdio.h>
#include <string.h>


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
		uint64_t Data =*(uint64_t*)(data + index);
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

int FLASHCORE :: getWarningThreshold()
{
	return _thresholds._warning;
}

int FLASHCORE :: getCriticalThreshold()
{
	return _thresholds._critical;
}

void FLASHCORE :: setWarningThreshold(int warning)
{
	_thresholds._warningDataWating = DATA_WAITING;
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

void FLASHCORE :: setCriticalThreshold(int critical)
{
	_thresholds._criticalDataWating = DATA_WAITING;
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

