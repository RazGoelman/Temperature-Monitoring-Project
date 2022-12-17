/*
 * Btn.cpp

 */
#include "Btn.h"
#include <stdio.h>


_buttonState Btn :: getState()  {return _statebut;}
void Btn :: setState			(_buttonState _state){ _statebut = _state;}
