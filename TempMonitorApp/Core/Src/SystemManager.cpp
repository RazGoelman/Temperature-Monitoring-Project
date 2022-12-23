/*
 * SystemManager.cpp

 */

#include <iostream>
#include <math.h>
#include "main.h"
#include "Btn.h"
#include "Dht.h"
#include "Buzzer.h"
#include "Rtc.h"
#include "Led.h"
#include "cmsis_os.h"
#include "Cli.h"
#include "Flash.h"
#include <stdio.h>
#include <stdlib.h>
#include "SystemManager.h"
#include "commTask.h"
#include "fatfs.h"
#include "ff.h"


extern  TIM_HandleTypeDef htim6;
extern  TIM_HandleTypeDef htim3;
extern  I2C_HandleTypeDef hi2c1;


// Initialized parameters
Btn button 					= 	Btn(SW1_GPIO_Port, SW1_Pin);
Dht dht 					= 	Dht(DHT_GPIO_Port, DHT_Pin, &htim6);
LED ledblue 				= 	LED(ledB_GPIO_Port, ledB_Pin);
LED ledred 					= 	LED(ledR_GPIO_Port, ledR_Pin);
BUZZER buzzer 				= 	BUZZER(&htim3);
_RTC rtc 					= 	_RTC(&hi2c1,0xD0);
FLASHCORE thresholdsFlash 	= 	FLASHCORE (THRESHOLDS_PAGE, 1);
CliContainer container 		= 	CliContainer();
CommTASK commtask 			=	CommTASK();

//variables for FatFs
FATFS 	FatFs; 	  //Fatfs handle
FIL 	fil; 	//File handle
FRESULT fres; //Result after operations
char 	Buf[100];



// Initialized SD card file
void sdInit()
{
	fres = f_mount(&FatFs, "", 1); //1=mount now
	if (fres != FR_OK) {
	printf("f_mount error (%i)\r\n", fres);
	}
	fres = f_open(&fil, "logger.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres == FR_OK) {
	printf("I was able to open 'logger.txt' for writing\r\n");
	}
	else {
	printf("f_open logger.txt error (%i)\r\n", fres);
	}
	//############################################################################
	fres = f_open(&fil, "logger2.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres == FR_OK) {
	printf("I was able to open 'logger2.txt' for writing\r\n");
	}
	else {
	printf("f_open logger2.txt error (%i)\r\n", fres);
	}

}
void systemManagerInit()
{
	thresholdsFlash.setCriticalThreshold(DEFAULT_TEMP);
	thresholdsFlash.setWarningThreshold(DEFAULT_TEMP);
	sdInit();
	HAL_TIM_Base_Init(&htim6);
	HAL_TIM_Base_Start_IT(&htim3);
	container.initCLIcontainer();
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	dht.onGpioInterrupt(GPIO_PIN_10);

	if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0){
		button.setState(BUTTON_PULLDOWN);
		dht.setState(TEMP_NO_BUZZER);
		buzzer.buzzerStopPlay();
	}
	button.setState(BUTTON_PULLUP);
}
// this task will start the cli option for the user.
extern "C" void StartcommTask(void *argument)
{
	/* Infinite loop */
	for (;;) {

		if (commtask.commTask()) {
			printf("\n---------- MENU ----------\r\n");
			printf("write 'help' to find a user options\r\n");

			commtask.handleCommand();
		}
	}
	osThreadTerminate(osThreadGetId());
}

// counter which manages the DHT task and write to SD card task
int mixcounter = xTaskGetTickCount();
// this task measure ever one second the temperature.
extern "C" void StartDht(void *argument)
{
	int Maxcounter = HAL_GetTick();
	osDelayUntil(mixcounter);
	while(1)
	{

		// The temperature reading failed
		if(dht.read() != HAL_OK)
		{
			dht.setState(TEMP_ERROR);
			printf("Error to read temperature\r\n");

		}
		else{
			/*change temperature sensor state
			  change  led state
			  change button
			  threshold  write into the threshold-log file
			  Normal temperature write into the normal log file
			  */

			if (dht.getTemp() >= thresholdsFlash.getCriticalThreshold()) {
					dht.setState(TEMP_CRITICAL);
					ledblue.Off();
					ledred.Blink();
					buzzer.buzzerStartPlay();
			}
			else if(dht.getTemp() >= thresholdsFlash.getWarningThreshold()){
					dht.setState(TEMP_WARNING);
					ledblue.Off();
					ledred.On();
					buzzer.buzzerStopPlay();
			}
			else {
				if(Maxcounter == ONE_MINUTES){
					dht.setState(TEMP_NORMAL);
					buzzer.buzzerStopPlay();
					ledblue.Blink();
					ledred.Off();
					Maxcounter = 0;
				}
					buzzer.buzzerStopPlay();
					ledblue.On();
					ledred.Off();
			}
		}
		osDelayUntil((mixcounter)+ONE_SECOND);

	}
	//Required to exit the task function osThreadTerminate must be used
	osThreadTerminate(osThreadGetId());
}

// this task will write the thresholds into the file.
extern "C" void StartFlashTask(void *argument)
{
	for(;;){
		DateTime * mytime = new DateTime;
		if (dht.getState() == TEMP_CRITICAL){
			sprintf(Buf,
					"Date: %02d/%02d/%02d, Time: %02d:%02d:%02d, Temperature: %.2f, Critical event\n\r",
					mytime->day, mytime->month, mytime->year, mytime->hours,
					mytime->min, mytime->sec, dht.getTemp());
					rtc.writeToThresholdFileSD(Buf);
					osDelay(10000);
		}
		else if (dht.getTemp() == TEMP_WARNING){
			sprintf(Buf,
					"Date: %02d/%02d/%02d, Time: %02d:%02d:%02d, Temperature: %.2f, Warning event\n\r",
					mytime->day, mytime->month, mytime->year, mytime->hours,
					mytime->min, mytime->sec, dht.getTemp());
					rtc.writeToThresholdFileSD(Buf);
					osDelay(10000);
		}
		else if (dht.getTemp() == TEMP_NORMAL){
			sprintf(Buf,
					"Date: %02d/%02d/%02d, Time: %02d:%02d:%02d, Temperature: %.2f, Normal Temperature\n\r",
					mytime->day, mytime->month, mytime->year, mytime->hours,
					mytime->min, mytime->sec, dht.getTemp());
					rtc.writeToNormalFileSD(Buf);
					osDelay(10000);
		}

	}
	osThreadTerminate(osThreadGetId());

}


