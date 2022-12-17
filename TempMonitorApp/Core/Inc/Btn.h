/*
 * Btn.h

 */

#ifndef INC_BTN_H_
#define INC_BTN_H_

#include "main.h"

enum _buttonState
{
	BUTTON_PULLUP,
	BUTTON_PULLDOWN,
};
class Btn
{
private:
	GPIO_TypeDef*   _gpioPort;
	uint16_t 		_gpioPin;
	_buttonState    _statebut;


public:
	Btn(GPIO_TypeDef* gpioPort, uint16_t gpioPin)
	{
		_gpioPort = gpioPort;
		_gpioPin  = gpioPin;
		_statebut = BUTTON_PULLUP;
	}
	_buttonState _state = 	BUTTON_PULLUP;
	_buttonState getState();

	void setState			(_buttonState _state);
	int getPin()			{return _statebut;}


};
#endif /* INC_BTN_H_ */
