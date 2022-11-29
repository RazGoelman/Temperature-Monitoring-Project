/*
 * Flash.h

 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "main.h"

#define DATA_WAITING 1
#define DEFAULT_TEMP 50
#define THRESHOLDS_PAGE_256 0x08080000
#define FLASH_BANK 2


struct THRESHOLDS
{
	uint16_t _warning;
	uint16_t _critical;
	uint16_t _criticalTempThreshold;
	uint16_t _warningTempThreshold;
};

class FLASHCORE
{
private:
	uint32_t _bank;
	uint32_t _pageAddr;
	uint32_t _nbPages;
	uint32_t _typeProgram;
	THRESHOLDS _thresholds;
public:
	FLASHCORE(uint32_t bank, uint32_t pageAddr, uint32_t nbPage, uint32_t typeProgram)
	{
		_bank = bank;
		_pageAddr = pageAddr;
		_nbPages = nbPage;
		_typeProgram = typeProgram;
	}

	HAL_StatusTypeDef erasePage();
	HAL_StatusTypeDef writeToPage(void* data, int dataSize);
	uint32_t getBank(){return _bank;}
	uint32_t getPageAddr(){return _pageAddr;}
	uint32_t getNbPages(){return _nbPages;}
	uint32_t getTypeProgram(){return _typeProgram;}
	int getWarningThreshold();
	int getCriticalThreshold();
	void setWarningThreshold(int warning);
	void setCriticalThreshold(int critical);
	void printThresHoldsTemperature();
	void SDDATA();

};


#endif /* INC_FLASH_H_ */
