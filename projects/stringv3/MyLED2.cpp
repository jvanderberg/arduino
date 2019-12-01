// include description files for other libraries used (if any)

/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "MyLED2.h"
#include <FastLED.h>

MyLED::MyLED(long speed, int num_leds)
{
	_speed = speed;
	_num_leds = num_leds;
}

void MyLED::xmas()
{

	return;
}