	/*
 * Dht.h

 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "main.h"



enum _alartState{
							TEMP_ERROR,
							TEMP_NORMAL,
							TEMP_WARNING,
							TEMP_CRITICAL,
							TEMP_NO_BUZZER
};
class Dht
{
private:
	GPIO_TypeDef * 			_gpioPort;
	uint16_t 				_gpioPin;
	TIM_HandleTypeDef * 	_timer;
	uint8_t 				_data[5];
	double 					_temperature;
	_alartState 			_dhtstate;
	int 					_counter;
	int 					_maxCounter;

public:

	Dht(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer)
	{
		_gpioPort 			= gpioPort;
		_gpioPin 			= gpioPin;
		_timer 				= timer;
		_temperature 		= 0.0;
		_dhtstate 			= TEMP_NORMAL;
		_counter			= 0;
		_maxCounter			= 0;
	}

	bool waitWhileEqual 	(int value, int expectedTime);
	void setGpioOutput();
	void setGpioInput();
	void setGpioExti();

	void setState			(_alartState _state);
	int read();
	double getTemp()		{return _temperature;}
	_alartState getState()	{return _dhtstate;}

	void onGpioInterrupt(uint16_t pin);

};


#endif /* INC_DHT_H_ */
