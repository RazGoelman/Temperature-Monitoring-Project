/*
 * Led.cpp

 */


#include "Led.h"


LED::LED(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin):
		GPIOx(GPIOx),GPIO_Pin(GPIO_Pin)
{
}

void LED::Led_On()
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

}
void LED::Led_Off()
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

}
void LED::LED_delay(int num){
	delay = num;
}
void LED::Led_Blink()
{
	HAL_GPIO_TogglePin(GPIOx,GPIO_Pin);
	HAL_Delay(delay);

}
