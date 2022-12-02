/*
 * Led.h

 */

#ifndef INC_LED_H_
#define INC_LED_H_

#define DEFAULT_PERIOD 	500

#include "main.h"

typedef enum _LED_STATE{
	LED_STATE_OFF,
	LED_STATE_ON,
	LED_STATE_BLINK
}LED_STATE;

class LED {
private:
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	int blinkcount = 0;
	int delay = 500 ;

	LED_STATE STATE;

public:
	LED(GPIO_TypeDef* GPIOx,
	uint16_t GPIO_Pin);

	void Led_On();
	void Led_Off();
	void LED_delay(int num);
	void Led_Blink();
	int Is_blink();
	void LEDchangeState();
};

#endif /* INC_LED_H_ */
