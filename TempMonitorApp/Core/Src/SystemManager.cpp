/*
 * MonitorManager.cpp

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
Btn button = 				Btn(SW1_GPIO_Port, SW1_Pin);
Dht dht = 	 				Dht(DHT_GPIO_Port, DHT_Pin, &htim6);
LED ledblue = 				LED(ledB_GPIO_Port, ledB_Pin);
LED ledred = 				LED(ledR_GPIO_Port, ledR_Pin);
BUZZER buzzer = 			BUZZER(&htim3);
_RTC rtc = 					_RTC(&hi2c1,0xD0);
FLASHCORE thresholdsFlash = FLASHCORE (THRESHOLDS_PAGE, 1);
CliContainer container = 	CliContainer();


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
		printf("f_open error (%i)\r\n", fres);
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
		dht.Dht_onGpioInterrupt(GPIO_PIN_10);

		if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0){
			dht.setState(TEMP_NO_BUZZER);
			thresholdsFlash.setCriticalThreshold(DEFAULT_TEMP);
			buzzer.buzzerStopPlay();
		}
}
// this task will start the cli option for the user.
extern "C" void StartcommTask(void *argument)
{
	/* Infinite loop */
		for (;;) {

			if (commTask()) {
					printf("\n---------- MENU ----------\r\n");
					printf("write 'help' to find a user options\r\n");

					handleCommand();
			}
		}
		osThreadTerminate(osThreadGetId());
}
// this task measure ever one second the temperature.
extern "C" void StartDht(void *argument)
{
	while(1)
	{
		// The temperature reading failed
		if(dht.Dht_read() != HAL_OK)
		{
			dht.setState(TEMP_ERROR);
			printf("Error to read temperature\r\n");

		}
		else{
			/*change temperature sensor state
			  change  led state
			  change button
			  threshold  write into the log file*/

			//thresholdsFlash.getCriticalThreshold()
			if (dht.getTemp() >= thresholdsFlash.getCriticalThreshold()) {
					dht.setState(TEMP_CRITICAL);
					ledblue.Led_Off();
					button.setState(BUTTON_PULLUP);
					ledred.Led_Blink();
					buzzer.buzzerStartPlay();
			}
			else if(dht.getTemp() >= thresholdsFlash.getWarningThreshold()){
					dht.setState(TEMP_WARNING);
					button.setState(BUTTON_PULLDOWN);
					ledblue.Led_Off();
					ledred.Led_On();
					buzzer.buzzerStopPlay();
			}
			else {
					dht.setState(TEMP_NORMAL);
					button.setState(BUTTON_PULLDOWN);
					buzzer.buzzerStopPlay();
					ledblue.Led_Blink();
					ledred.Led_Off();
			}
		}
		osDelay(ONE_SECOND);

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
						rtc.writeToFileSD(Buf);
						osDelay(10000);
		}
		else if (dht.getTemp() == TEMP_WARNING){
			sprintf(Buf,
						"Date: %02d/%02d/%02d, Time: %02d:%02d:%02d, Temperature: %.2f, Warning event\n\r",
						mytime->day, mytime->month, mytime->year, mytime->hours,
						mytime->min, mytime->sec, dht.getTemp());
						rtc.writeToFileSD(Buf);
						osDelay(10000);
		}
		else{
			sprintf(Buf,
						"Date: %02d/%02d/%02d, Time: %02d:%02d:%02d, Temperature: %.2f, Normal Temperature\n\r",
						mytime->day, mytime->month, mytime->year, mytime->hours,
						mytime->min, mytime->sec, dht.getTemp());
						rtc.writeToFileSD(Buf);
						osDelay(10000);
		}


	}
	osThreadTerminate(osThreadGetId());

}


