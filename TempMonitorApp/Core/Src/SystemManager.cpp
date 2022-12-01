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


extern  TIM_HandleTypeDef htim6;
extern  TIM_HandleTypeDef htim3;
extern  I2C_HandleTypeDef hi2c1;

double warningThreshold = 27.00;
double criticalThreshold = 29.00;

Btn button = Btn(SW1_GPIO_Port, SW1_Pin);
Dht dht = Dht(DHT_GPIO_Port, DHT_Pin, &htim6);
CliContainer container = CliContainer();
LED ledblue = LED(ledB_GPIO_Port, ledB_Pin);
LED ledred = LED(ledR_GPIO_Port, ledR_Pin);
BUZZER buzzer = BUZZER(&htim3);
FLASHCORE thresholdsFlash = FLASHCORE (DATA_WAITING, THRESHOLDS_PAGE_256, 1, FLASH_TYPEPROGRAM_DOUBLEWORD);
_RTC rtc = _RTC(&hi2c1,0xD0);

//some variables for FatFs
FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations
BYTE readBuf[30];


void SD_init()
{
		fres = f_mount(&FatFs, "", 1); //1=mount now
		if (fres != FR_OK) {
		printf("f_mount error (%i)\r\n", fres);
		while(1);
		}
		//Now let's try and write a file "write.txt"
		fres = f_open(&fil, "logger.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
		if(fres == FR_OK) {
		printf("I was able to open 'logger.txt' for writing\r\n");
		} else {
		printf("f_open error (%i)\r\n", fres);
		}
}



void managerInit()
{
	HAL_TIM_Base_Init(&htim6);
	HAL_TIM_Base_Start_IT(&htim3);
	container.initCLIcontainer();
	SD_init();
	thresholdsFlash.printThresHoldsTemperature();
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		dht.Dht_onGpioInterrupt(GPIO_Pin);

		if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)){
			dht.setState(TEMP_NO_BUZZER);
			buzzer.buzzerStopPlay();
		}

}

// this task will start the cli option for the user.
extern "C" void StartcommTask(void *argument)
{
	/* Infinite loop */
		for (;;) {

			if (commTask()) {
				handleCommand();
			}
		}
		osThreadTerminate(osThreadGetId());
}
// this task measure ever one second the temp.
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
			if (dht.getTemp() >= criticalThreshold) {
					dht.setState(TEMP_CRITICAL);
					ledblue.Led_Off();
					button.setState(BUTTON_PULLUP);
					ledred.Led_Blink();
					buzzer.buzzerStartPlay();
					//utoa(dht.getTemp(),(char*)readBuf,20);
					//printf("temp is %.2f \r\n", dht.getTemp());
			}
			else if(dht.getTemp() >= warningThreshold){
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
					ledblue.Led_On();
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

		if (dht.getState() == TEMP_CRITICAL){

			printf("\r\n~  SD card  ~\r\n\r\n");

			HAL_Delay(1000); //a short delay is important to let the SD card settle

			//Open the file system
			fres = f_mount(&FatFs, "", 1); //1=mount now
			if (fres != FR_OK) {
			printf("f_mount error (%i)\r\n", fres);
			while(1);
			}
			//Now let's try and write a file "write.txt"
			fres = f_open(&fil, "logger.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
			if(fres == FR_OK) {
			printf("I was able to open 'logger.txt' for writing\r\n");
			} else {
			printf("f_open error (%i)\r\n", fres);
			}

			//Copy in a string
			// write temperature
			utoa(dht.getTemp(),(char*)readBuf,20);
			UINT bytesWrote;
			fres = f_write(&fil, readBuf, 19, &bytesWrote);
			//////////////////////////////////////////////
			//write date time
			//utoa(rtc.rtcGetTime(),(char*)readBuf,20);
			//UINT bytesWrote;
			fres = f_write(&fil, readBuf, 19, &bytesWrote);
			//////////////////////////////////////////////
			if(fres == FR_OK) {
			printf("Wrote %i bytes to 'logger.txt'!\r\n", bytesWrote);
			} else {
			printf("f_write error (%i)\r\n", fres);
			}

			//Be a tidy kiwi - don't forget to close your file!
			f_close(&fil);

			//We're done, so de-mount the drive
			f_mount(NULL, "", 0);

		}

	}
	osThreadTerminate(osThreadGetId());

}


