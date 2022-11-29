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

extern Dht dht;
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
class SetWarning : public Cli
{
private:
	FLASHCORE* _flash;
public:
	SetWarning(FLASHCORE* flash){
		_flash = flash;

	}
	void doCommand(char* param);

};

class SetCritical : public Cli
{
private:
	FLASHCORE* _flash;
public:
	void doCommand(char* param);
};
class GetFlashInfo : public Cli
{
private:
	FLASHCORE* _flash;
public:
	void doCommand(char* param){
		_flash->printThresHoldsTemperature();
		printf("יTemprature = %.2f \r\n", dht.getTemp());
	}

};





#endif /* INC_CLI_H_ */
