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
	GPIO_TypeDef * _gpioPort;
	uint16_t _gpioPin;
	TIM_HandleTypeDef * _timer;
	uint8_t _data[5];
	double _temperature;
	_alartState _dhtstate;

public:

	Dht(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer)
	{
		_gpioPort = gpioPort;
		_gpioPin = gpioPin;
		_timer = timer;
		_temperature = 0.0;
		_dhtstate = TEMP_NORMAL;
	}

	bool waitWhileEqual (int value, int expectedTime);
	void setGpioOutput();
	void setGpioInput();
	_alartState getState();
	void setState(_alartState _state);
	int Dht_read();
	double getTemp();
	void Dht_onGpioInterrupt(uint16_t pin);

};


#endif /* INC_DHT_H_ */
