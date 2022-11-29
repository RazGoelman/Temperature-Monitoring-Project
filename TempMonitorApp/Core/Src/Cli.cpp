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
extern FLASHCORE thresholdsFlash;


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
                _flash->setWarningThreshold(value);
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
                _flash->setCriticalThreshold(value);
        }
}





void CliContainer::initCLIcontainer(){

	//set / get time to the RTC
	container.RegisterCommand("set-time",new rtcsettime(&rtc));
	container.RegisterCommand("get-time",new rtcgettime(&rtc));

	//set warning / critical temperature
	//container.RegisterCommand("set-warning",SetWarning());
	//container.RegisterCommand("set-critical", new SetCritical());
	//container.RegisterCommand("print", new thresholdsFlash.printThresHoldsTemperature());
	// switch on / off led
	container.RegisterCommand("led-on",new ledOn(&ledblue));
	container.RegisterCommand("led-off",new ledOff(&ledblue));
	container.RegisterCommand("led-blink",new ledBlink(&ledblue));

	//play / stop analog buzzer
	container.RegisterCommand("play",new buzzeron(&buzzer));
	container.RegisterCommand("stop",new buzzeroff(&buzzer));

	container.RegisterCommand("rtc-start",new rtcstart(&rtc));
	container.RegisterCommand("rtc-stop",new rtcstop(&rtc));




}
