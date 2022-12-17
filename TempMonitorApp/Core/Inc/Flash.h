/*
 * Flash.h

 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "main.h"
#include "integer.h"
#include "ff.h"
#define DATA_WAITING 	1
#define DEFAULT_TEMP 	50
#define THRESHOLDS_PAGE 0x08080000
#define FLASH_BANK 		2
#define PAGE_SIZE 		2048
#define MAX_COMMANDS   	20

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
						uint32_t _page;
						uint32_t _typeProgram;
						THRESHOLDS _thresholds;
public:
	FLASHCORE(uint32_t pageAddr, uint32_t nbPage)
	{
		_pageAddr = 	pageAddr;
		_page = 		(_pageAddr - 0x08000000) / PAGE_SIZE;
		_bank = 		_page < 256 ? 1 : 2;
		_nbPages = 		nbPage;
		_typeProgram = 	FLASH_TYPEPROGRAM_DOUBLEWORD;
	}
	HAL_StatusTypeDef erasePage();
	HAL_StatusTypeDef writeToPage(void* data, int dataSize);

	uint32_t getBank()			{return _bank;}
	uint32_t getPageAddr()		{return _pageAddr;}
	uint32_t getNbPages()		{return _nbPages;}
	uint32_t getTypeProgram()	{return _typeProgram;}


	int getWarningThreshold()	{return _thresholds._warning;}
	int getCriticalThreshold();
	void setWarningThreshold(int warning);
	void setCriticalThreshold(int critical);
	void SDData();
	void removeFileFromSD();

};

#endif /* INC_FLASH_H_ */
