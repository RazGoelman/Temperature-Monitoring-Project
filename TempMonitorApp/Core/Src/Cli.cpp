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


extern LED 			ledblue;
extern int 			blinkOn;
extern CliContainer container;
extern BUZZER 		buzzer;
extern _RTC 		rtc;
extern Dht 			dht;
extern FLASHCORE 	thresholdsFlash;

//show all the command line options
void CliContainer::PrintCommand()
{
	commands->CommandName[COMMAND_NAME_SIZE];
	memset(&commands, 0, sizeof(commands));
	int _commandsCount = 0;
	printf("Available commands:\r\n");
		for (int i = 0; i < _commandsCount; i++) {
			printf("\t%s\r\n", commands[i].CommandName);
		}
}

void CliContainer::initCLIcontainer(){

	//set / get time to the RTC
	container.RegisterCommand("set-date-time",		new rtcsettime(&rtc));
	container.RegisterCommand("get-date-time",		new rtcgettime(&rtc));

	//set warning / critical, threshold temperature
	container.RegisterCommand("set-warning", 		new WarningTempThreshold());
	container.RegisterCommand("set-critical", 		new CriticalTempThreshold());
	container.RegisterCommand("get-threshold-info", new GetTempThresholdInfo());

	//get some statistics from the SD card
	container.RegisterCommand("get-sd-data", 		new PrintSDData());
	//print SD data to the screen
	container.RegisterCommand("read-sd-data", 		new readSDCard(&rtc));

	//Remove file from SD card
	container.RegisterCommand("clear-file", 		new RemoveFileSDCard());

	// switch on / off led
	container.RegisterCommand("led-on",				new ledOn(&ledblue));
	container.RegisterCommand("led-off",			new ledOff(&ledblue));
	container.RegisterCommand("led-blink",			new ledBlink(&ledblue));

	//play / stop analog buzzer
	container.RegisterCommand("play-buzzer",		new buzzeron(&buzzer));
	container.RegisterCommand("stop-buzzer",		new buzzeroff(&buzzer));
	// help command to see all the command line options
	container.RegisterCommand("help", 				new helpCMD());


}
