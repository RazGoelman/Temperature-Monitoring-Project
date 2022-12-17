/*
 * Led.h

 */

#ifndef INC_LED_H_
#define INC_LED_H_

#define DEFAULT_PERIOD 		50

#include "main.h"

enum _LED_STATE{
							LED_STATE_OFF,
							LED_STATE_ON,
							LED_STATE_BLINK
};

class LED {
private:
	GPIO_TypeDef* 			GPIOx;
	uint16_t 				GPIO_Pin;
	int 					blinkcount;
	int 					delay;
	_LED_STATE 				STATE;

public:
	LED(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

	void On();
	void Off();
	void Delay(int num);
	void Blink();
};

#endif /* INC_LED_H_ */
