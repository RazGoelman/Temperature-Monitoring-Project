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
#include "diskio.h"
#include <string.h>
#include "ff_gen_drv.h"
#include "ff.h"



extern LED ledblue;
extern int blinkOn;
extern CliContainer container;
extern BUZZER buzzer;
extern _RTC rtc;
extern Dht dht;
extern FLASHCORE thresholdsFlash;





void CliContainer::initCLIcontainer(){

	//set / get time to the RTC
	container.RegisterCommand("set-time",new rtcsettime(&rtc));
	container.RegisterCommand("get-time",new rtcgettime(&rtc));

	//set warning / critical temperature
	container.RegisterCommand("warning", new WarningTempThreshold());
	container.RegisterCommand("critical", new CriticalTempThreshold());
	//container.RegisterCommand("print", new GetTempThresholdInfo());

	//get some statistics from the SD card
	container.RegisterCommand("SD-data", new PrintSDData());
	//Remove file from SD card
	container.RegisterCommand("clear", new RemoveFileSDCard());

	// switch on / off led
	container.RegisterCommand("led-on",new ledOn(&ledblue));
	container.RegisterCommand("led-off",new ledOff(&ledblue));
	container.RegisterCommand("led-blink",new ledBlink(&ledblue));

	//play / stop analog buzzer
	container.RegisterCommand("play",new buzzeron(&buzzer));
	container.RegisterCommand("stop",new buzzeroff(&buzzer));
	// time as get_fattime function
	container.RegisterCommand("time", new timeRTC());

	/*
	container.RegisterCommand("rtc-start",new rtcstart(&rtc));
	container.RegisterCommand("rtc-stop",new rtcstop(&rtc));
	*/



}
