/*
 * MonitorManager.cpp

 */

#include <iostream>
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
#include <SystemManager.h>
#include "commTask.h"


extern  TIM_HandleTypeDef htim6;
extern  TIM_HandleTypeDef htim3;
extern  I2C_HandleTypeDef hi2c1;

Btn button = Btn(SW1_GPIO_Port, SW1_Pin);
Dht dht = Dht(DHT_GPIO_Port, DHT_Pin, &htim6);
CliContainer container = CliContainer();
LED ledblue = LED(ledB_GPIO_Port, ledB_Pin);
LED ledred = LED(ledR_GPIO_Port, ledR_Pin);
BUZZER buzzer = BUZZER(&htim3);
FLASHCORE thresholdsFlash = FLASHCORE (DATA_WAITING, THRESHOLDS_PAGE_256, 1, FLASH_TYPEPROGRAM_DOUBLEWORD);
_RTC rtc = _RTC(&hi2c1,0xD0);





void managerInit()
{
	HAL_TIM_Base_Init(&htim6);
	HAL_TIM_Base_Start_IT(&htim3);
	container.initCLIcontainer();
	//thresholdsFlash->printThresHolds();

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	GPIOB -> MODER &= ~(GPIO_MODER_MODER1);
	GPIOB -> PUPDR &= ~(GPIO_PUPDR_PUPDR1);
	int counter =0;

	if((GPIOA -> IDR & (GPIO_IDR_ID1)) && counter == 0){
		dht.setState(TEMP_NO_BUZZER); // CHENGE DHT STAGE
		buzzer.buzzerStopPlay();	// STOP BUZZER
		counter = 1;

	}

}

// this task will start the cli option for the user.
extern "C" void StartcommTask(void *argument)
{
	/* Infinite loop */
		for (;;) {

			if (commTask()) {
				printf("Set date and time");
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
			if (dht.getTemp() >= thresholdsFlash.getCriticalThreshold()) {
						dht.setState(TEMP_CRITICAL);

						button.setState(BUTTON_PULLUP);
						//ledred.Led_Blink();
						buzzer.buzzerStartPlay();
						//printf("temp is %.2f \r\n", dht.getTemp());
			}
			else if(dht.getTemp() >= thresholdsFlash.getWarningThreshold()){
						dht.setState(TEMP_WARNING);
						button.setState(BUTTON_PULLDOWN);
						ledred.Led_On();
						buzzer.buzzerStopPlay();


			}
			else {
				dht.setState(TEMP_NORMAL);
				button.setState(BUTTON_PULLDOWN);
				buzzer.buzzerStopPlay();
				ledblue.Led_On();
			}
		}
		osDelay(ONE_SECOND);

	}
	//Required to exit the task function osThreadTerminate must be used
	osThreadTerminate(osThreadGetId());
}


extern "C" void StartLedTask(void *argument)
{
	for (;;) {
		if(dht.getState() == TEMP_CRITICAL){
			ledred.Led_Blink();
			osDelay(500);
		}

	}
	osThreadTerminate(osThreadGetId());

}
// this task will write the thresholds into the file.
extern "C" void StartFlashTask(void *argument)
{
	for(;;){
		if (dht.getState() == TEMP_WARNING){
			//thresholdsFlash.writeToPage(dht->getTemp(), dataSize)
			//printf("TEMP_WARNING");

		}

	}
	osThreadTerminate(osThreadGetId());
}



