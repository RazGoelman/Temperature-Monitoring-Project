/*
 * Btn.cpp
 *
 *  Created on: Nov 24, 2022
 *      Author: student
 */
#include "Btn.h"
#include <stdio.h>


_buttonState Btn :: getState(){return _dhtstate;}
	void Btn :: setState(_buttonState _state){ _dhtstate = _state;}
