/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef MyLED2_h
#define MyLED2_h
#include <FastLED.h>
#include "Arduino.h"
const CRGB WHITE = CRGB(255, 255, 255);
const CRGB PURPLE = CRGB(30, 0, 60);
const CRGB ORANGE = CRGB(80, 20, 0);
const CRGB BLACK = CRGB(0, 0, 0);
const CRGB LIGHT_BLUE = CRGB(50, 50, 200);
const CRGB PINK = CRGB(255, 30, 30);

template <int NUM_LEDS>
class MyLED
{
  public:
	MyLED(long speed, CRGBArray<NUM_LEDS> leds)
	{
		_speed = speed;
		_leds = leds;
	}
	void clear()
	{
		for (int i = 0; i < NUM_LEDS; i++)
		{
			_leds[i] = BLACK;
		}
		show();
	}
	void show()
	{
		FastLED.show();
	}

	boolean delay_and_check(long pause)
	{
		if (pause < 50)
		{
			delay(pause);
			return check_io();
		}
		for (long i = 0; i < pause; i += 50)
		{
			delay(50);
			//	Serial.println(i);
			if (check_io())
				return true;
		}
		return false;
	}
	void everyOther(CRGB one, CRGB two)
	{
		for (long i = 0; i < NUM_LEDS; i++)
		{
			if (i % 2 == 0)
			{
				_leds[i] = one;
			}
			else
			{
				_leds[i] = two;
			}
		}
	}
	boolean switchColor(long time, long pause, CRGB one, CRGB two)
	{
		for (long i = 0; i < time; i += 2 * pause)
		{
			if (check_io())
				return true;
			everyOther(one, two);
			show();
			if (delay_and_check(pause))
				return true;
			show();
			if (delay_and_check(pause))
				return true;
		}
	}
	void randomColor(CRGB colors[], int numColors)
	{
		int j = 0;
		for (long i = 0; i < NUM_LEDS; i++)
		{
			CRGB color;
			if (i == 0)
			{
				color = colors[random(0, numColors - 1)];
			}
			else
			{
				color = _leds[i - 1];
				while (color == _leds[i - 1])
				{
					long rand = random(0, numColors - 1);
					color = colors[rand];
				}
			}
			_leds[i] = color;
		}
		show();
	}
	void xmas()
	{

		CRGBArray<NUM_LEDS> d;
		d = _leds;
		boolean done = chase(10, _speed * 4l, 100, CRGB(255, 0, 0), CRGB(0, 255, 0));
		if (done)
			return;
		if (check_io())
			return;
		done = chase(10, _speed * 4l, 100, CRGB(255, 0, 0), CRGB(200, 200, 200));
		if (done)
			return;
		if (check_io())
			return;
		done = switchColor(_speed * 4l, 1000, CRGB(0, 0, 255), CRGB(200, 200, 200));
		if (done)
			return;

		if (check_io())
			return;
		CRGB randomColors[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};
		randomColor(randomColors, 5);
		if (check_io())
			return;
		return;
	}

	boolean chase(long length, long time, long pause, CRGB one, CRGB two);
	boolean check_io() {}

  protected:
	long _speed;
	CRGBArray<NUM_LEDS> _leds;
};

template <int NUM_LEDS>
boolean MyLED<NUM_LEDS>::chase(long length, long time, long pause, CRGB one, CRGB two)
{
	long i;
	for (long k = 0; k < time; k += pause)
	{
		i = i + 1;
		for (long j = 0; j < NUM_LEDS; j++)
		{
			long mod = (j + i) % (2 * length);
			if (mod < length)
			{
				_leds[j] = one;
			}
			else
			{
				_leds[j] = two;
			}
		}
		show();

		if (delay_and_check(pause))
		{
			return true;
		}
	}
}
#endif