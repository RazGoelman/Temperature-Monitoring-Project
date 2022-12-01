/*
 * Cli.h
 *

 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#define COMMAND_ARR_SIZE 10
#define COMMAND_NAME_SIZE 20

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

extern Dht dht;
extern double warningThreshold;
extern double criticalThreshold;
extern FLASHCORE thresholdsFlash;


class Cli{
public:
	virtual void doCommand(const char * param) = 0;
	virtual ~Cli(){}
};

class clicommand_entry {
public:
	const char * CommandName;
	Cli* command;
};

class CliContainer{
private:
	clicommand_entry commands[20];
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
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Led

class ledOn : public Cli{
private:
	LED * _led;
public:
	ledOn(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->Led_On();
	}
};
class ledOff : public Cli{
private:
	LED * _led;
public:
	ledOff(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->Led_Off();
	}
};
class ledBlink : public Cli{
private:
	LED * _led;
public:
	ledBlink(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->Led_Blink();
	}
};
class ledSetDelay : public Cli{
private:
	LED * _led;
public:
	ledSetDelay(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		int _param = atoi(param);
		_led->LED_delay(_param);
	}
};
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//RTC

class rtcstart : public Cli{
private:
	_RTC * _rtc;
public:
	rtcstart(_RTC * rtc){
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
	rtcgettime(_RTC * rtc){
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
	rtcsettime(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		char * token;
		char s[3] = ":";
		char temp[50];
		strcpy(temp,param);
		DateTime tempdate;

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
	rtcstop(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStop();
	}
};
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//BUZZER

class buzzeron : public Cli{
private:
	BUZZER * _buzzer;
public:
	buzzeron(BUZZER * buzzer){
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
	buzzeroff(BUZZER * buzzer){
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStopPlay();
	}
};
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//TRESHOLD FLASH
class WarningTempThreshold: public Cli {
private:
	double _warningTempThreshold;
public:
	WarningTempThreshold(){
		_warningTempThreshold = 50;
	}

	void doCommand(const char* param){
		_warningTempThreshold = atof(param);
		warningThreshold = _warningTempThreshold;
		thresholdsFlash.setWarningThreshold(warningThreshold);
		//thresholdsFlash.flash_write(THRESHOLDS_PAGE_256, warningThreshold, double __warningTempThreshold));
	}
};

class CriticalTempThreshold: public Cli {
private:
	double _criticalTempThreshold;
public:
	CriticalTempThreshold(){
		_criticalTempThreshold = 50;
	}

	void doCommand(const char* param){
		_criticalTempThreshold = atof(param);
		criticalThreshold = _criticalTempThreshold;
		thresholdsFlash.setCriticalThreshold(criticalThreshold);
	}
};

class GetTempThresholdInfo : public Cli
{
private:
	FLASHCORE* _flash;
public:
	GetTempThresholdInfo()
	{
		//_flash;
	}
	void doCommand(const char* param){
		_flash->printThresHoldsTemperature();
		printf("יTemprature = %.2f \r\n", dht.getTemp());
	}

};
 class PrintSDData : public Cli {
private:
	FLASHCORE* _flash;
public:
	PrintSDData(){
		//_flash;
	}
	void doCommand(const char* param){
		_flash->SDDATA();
	}
};

class RemoveFileSDCard : public Cli {
private:
	 FLASHCORE* _flash;
public:
	 RemoveFileSDCard(){

	 }
	 void doCommand(const char* param){

	 }


private:
};

class timeRTC : public Cli
{
private:
	 FLASHCORE* _flash;
public:
	 timeRTC(){

	 }
	 void doCommand(const char* param){
	 		_flash->TIMERTC();
	 	}

};


#endif /* INC_CLI_H_ */
