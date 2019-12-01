/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef MyLED2_h
#define MyLED2_h
#include <FastLED.h>
#include "Arduino.h"

class MyLED
{
  public:
	MyLED(long speed, int num_leds);
	void xmas();
	virtual boolean check_io();

  private:
	long _speed;
	int _num_leds;
};

#endif