/*
 * Cli.cpp

 */
#include "Cli.h"
#include "stdlib.h"
#include "Dht.h"
#include "Rtc.h"
#include <stdio.h>
#include "Buzzer.h"
#include "Led.h"


extern LED ledblue;
extern int blinkOn;
extern CliContainer container;
extern BUZZER buzzer;
extern _RTC rtc;
extern Dht dht;

extern Flash* thresholdsFlash;


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

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void SetWarning :: doCommand(char* param)
{
	if(param == nullptr)
	{
		printf("setFlashInfo's doCommand with param %s\r\n", param);
	}
	else
	{
		int value = atoi(param);
		_flash->setWarning(value);
	}
}

void SetCritical :: doCommand(char* param)
{
	if(param == nullptr)
	{
		printf("setFlashInfo's doCommand with param %s\r\n", param);
	}
	else
	{
		int value = atoi(param);
		_flash->setCritical(value);
	}
}

void GetFlashInfo :: doCommand(char* param)
{

	_flash->printThresHolds();
	printf("temp = %.2f \r\n", dht.getTemp());
}


void CliContainer::initCLIcontainer(){
	container.RegisterCommand("led-on",new ledOn(&ledblue));
	container.RegisterCommand("led-off",new ledOff(&ledblue));
	container.RegisterCommand("led-blink",new ledBlink(&ledblue));
	container.RegisterCommand("set-time",new rtcsettime(&rtc));
	container.RegisterCommand("get-time",new rtcgettime(&rtc));
	container.RegisterCommand("rtc-start",new rtcstart(&rtc));
	container.RegisterCommand("rtc-stop",new rtcstop(&rtc));
	container.RegisterCommand("play",new buzzeron(&buzzer));
	container.RegisterCommand("stop",new buzzeroff(&buzzer));

//	SetWarning* setWarning = new SetWarning("setWarning", thresholdsFlash);
//
//	SetCritical* setCritical = new SetCritical("setCritical", thresholdsFlash);
//
//	GetFlashInfo* getFlashInfo = new GetFlashInfo("getInfo", thresholdsFlash);


}
