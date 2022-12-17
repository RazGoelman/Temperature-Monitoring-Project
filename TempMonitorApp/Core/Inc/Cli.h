/*
 * Cli.h
 *
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_



#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Flash.h"
#include "Buzzer.h"
#include "Led.h"
#include "Rtc.h"
#include "Dht.h"
#include "user_diskio.h"
#include "integer.h"

#define COMMAND_ARR_SIZE 	10
#define COMMAND_NAME_SIZE 	20
extern Dht 					dht;
extern FLASHCORE 			thresholdsFlash;


class Cli{
public:
	virtual void doCommand(const char * param) = 0;
	virtual ~Cli(){}
};

struct  _clicommand_entry
{
	const char * CommandName;
	Cli* command;
};

class CliContainer{
private:
	_clicommand_entry commands[20];
	int funcounter = 0;
public:
	CliContainer(){}
	~CliContainer(){}
	void RegisterCommand(const  char* CommandName, Cli * clicommand) {
		commands[funcounter].CommandName = CommandName;
		commands[funcounter].command = clicommand;
		funcounter++;
	}

	void doCommand( char * cmd,  char * param){
		for (int j = 0; j <funcounter; j++) {
			if (strcmp(cmd,commands[j].CommandName) == 0) {
				commands[j].command->doCommand(param);
				return;
			}

		}
		printf("Invalid command\r\n");
	}
	void initCLIcontainer();
	void PrintCommand();
};
/////////////////////////////////////////////////////////////////////////////////
//Led
/////////////////////////////////////////////////////////////////////////////////
class ledOn : public Cli{
private:
	LED * _led;
public:
	ledOn(LED * led)
	{
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->On();
	}
};

class ledOff : public Cli{
private:
	LED * _led;
public:
	ledOff(LED * led)
	{
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->Off();
	}
};

class ledBlink : public Cli{
private:
	LED * _led;
public:
	ledBlink(LED * led)
	{
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->Blink();
	}
};

class ledSetDelay : public Cli{
private:
	LED * _led;
public:
	ledSetDelay(LED * led)
	{
		_led = led;
	}
	void doCommand(const char * param) override{
		int _param = atoi(param);
		_led->Delay(_param);
	}
};
/////////////////////////////////////////////////////////////////////////////
//RTC
//////////////////////////////////////////////////////////////////////////////
class rtcstart : public Cli{
private:
	_RTC * _rtc;
public:
	rtcstart(_RTC * rtc)
	{
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStart();
	}
};

class rtcgettime : public Cli{
private:
	_RTC * _rtc;
public:
	rtcgettime(_RTC * rtc)
	{
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcGetTime();

	}
};

class rtcsettime : public Cli{
private:
	_RTC * _rtc;
public:
	rtcsettime(_RTC * rtc)
	{
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		char * 		token;
		char 		s[3] = ":";
		char 		temp[50];
		strcpy(temp,param);
		_DateTime 	tempdate;

		token = strtok(temp, s);
		tempdate.hours = atoi(token);
		token = strtok(NULL, s);

		tempdate.min = atoi(token);
		token = strtok(NULL, s);

		tempdate.sec = atoi(token);
		token = strtok(NULL, s);

		tempdate.weekDay = atoi(token);
		token = strtok(NULL, s);

		tempdate.day = atoi(token);
		token = strtok(NULL, s);

		tempdate.month = atoi(token);
		token = strtok(NULL, s);

		tempdate.year = atoi(token);
		token = strtok(NULL, s);




		_rtc->rtcSetTime(&tempdate);
	}
};

class rtcstop : public Cli{
private:
	_RTC * _rtc;
public:
	rtcstop(_RTC * rtc)
	{
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStop();
	}
};

class readSDCard : public Cli{
private:
	_RTC * _rtc;
public:
	readSDCard(_RTC * rtc)
	{
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->readFileFromSD();
	}

private:
};
//////////////////////////////////////////////////////////////
//BUZZER
//////////////////////////////////////////////////////////////

class buzzeron : public Cli{
private:
	BUZZER * _buzzer;
public:
	buzzeron(BUZZER * buzzer)
	{
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStartPlay();
	}
};

class buzzeroff : public Cli{
private:
	BUZZER * _buzzer;
public:
	buzzeroff(BUZZER * buzzer)
	{
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStopPlay();
	}
};
///////////////////////////////////////////////////////////////////
//TRESHOLD FLASH
//////////////////////////////////////////////////////////////////
class WarningTempThreshold: public Cli {
private:
	double _warningTempThreshold;
public:
	WarningTempThreshold()
	{
		_warningTempThreshold = 50;
	}

	void doCommand(const char* param) {
		_warningTempThreshold = atof(param);
		//warningThreshold = _warningTempThreshold;
		thresholdsFlash.setWarningThreshold(_warningTempThreshold);
		//thresholdsFlash.flash_write(THRESHOLDS_PAGE_256, warningThreshold, double __warningTempThreshold));
	}
};

class CriticalTempThreshold: public Cli {
private:
	double _criticalTempThreshold;
public:
	CriticalTempThreshold()
	{
		_criticalTempThreshold = 50;
	}

	void doCommand(const char* param) {
		_criticalTempThreshold = atof(param);
		//criticalThreshold = _criticalTempThreshold;
		thresholdsFlash.setCriticalThreshold(_criticalTempThreshold);
	}
};

// print warning / critical data
class GetTempThresholdInfo : public Cli
{
private:
	FLASHCORE* _flash;
public:
	GetTempThresholdInfo()
	{
	}
	void doCommand(const char* param) {
		if ( thresholdsFlash.getWarningThreshold() == DEFAULT_TEMP){
		        	printf("Please insert critical temperature\r\n");
		}
		else{
			printf("\nCritical = %d \r\n", thresholdsFlash.getCriticalThreshold());
		}
		if(thresholdsFlash.getWarningThreshold() == DEFAULT_TEMP){
			printf("\nPlease insert warning temperature\r\n");
		}
		else{
			printf("\nWarning = %d \r\n", thresholdsFlash.getWarningThreshold());
		}
	}

};
//////////////////////////////////////////////////////////////////////
//SD Card
/////////////////////////////////////////////////////////////////////
 class PrintSDData : public Cli {
private:
	FLASHCORE* _flash;
public:
	PrintSDData()
	{
	}
	void doCommand(const char* param) override{
		_flash->SDData();
	}
};

class RemoveFileSDCard : public Cli {
private:
	 FLASHCORE* _flash;
public:
	 RemoveFileSDCard()
	 {
	 }
	 void doCommand(const char* param) override{
		 _flash->removeFileFromSD();
	 }


private:
};
//////////////////////////////////////////////////////////////////////////
// help
/////////////////////////////////////////////////////////////////////////
class helpCMD : public Cli {
private:
	CliContainer* commands;
public:
	helpCMD()
	{
	}
	void doCommand(const char* param) override{
		commands->PrintCommand();
	}
};
#endif /* INC_CLI_H_ */
