/*
 * Buzzer.cpp

 */

#include "main.h"
#include "Buzzer.h"

extern TIM_HandleTypeDef htim3;


BUZZER::BUZZER(TIM_HandleTypeDef *htim)
{
	_htim 	= htim;

}
void BUZZER::buzzerStartPlay()
{
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_AUTORELOAD(&htim3,150);
}
void BUZZER::buzzerStopPlay()
{
	HAL_TIM_Base_Stop(&htim3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}
