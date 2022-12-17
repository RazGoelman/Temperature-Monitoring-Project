/*
 * Led.cpp

 */


#include "Led.h"


LED::LED(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin):
		GPIOx(GPIOx),GPIO_Pin(GPIO_Pin)
{
	blinkcount  = 0;
	delay 		= 50;
}

void LED::On()
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

}
void LED::Off()
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

}

void LED::Delay(int num){
	delay = num;
}

void LED::Blink()
{
	HAL_GPIO_TogglePin(GPIOx,GPIO_Pin);
	HAL_Delay(delay);

}
