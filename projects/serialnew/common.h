#ifndef common_h
#define common_h
#include <FastLED.h>
#include "Arduino.h"
#define DEBUG 1



void debug(char *s)
{
#ifdef DEBUG
	Serial.println(s);
#endif
}

void debug(long l)
{
#ifdef DEBUG
	Serial.println(l);
#endif
}

void debug(int i)
{
#ifdef DEBUG
	Serial.println(i);
#endif
}

void debug(double d)
{
#ifdef DEBUG
	Serial.println(d);
#endif
}

void debug(boolean b)
{
#ifdef DEBUG
	Serial.println(b);
#endif
}

void show()
{
	FastLED.show();
}

void clear()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	show();
}
void everyOther(CRGB one, CRGB two)
{
	for (long i = 0; i < NUM_LEDS; i++)
	{
		if (i % 2 == 0)
		{
			leds[i] = one;
		}
		else
		{
			leds[i] = two;
		}
	}
	show();
}

void everyFourth(CRGB one, CRGB two, CRGB three, CRGB four)
{
	int j = 0;
	for (long i = 0; i < NUM_LEDS; i++)
	{

		if (j == 0)
		{
			leds[i] = one;
		}
		else if (j == 1)
		{
			leds[i] = two;
		}
		else if (j == 2)
		{
			leds[i] = three;
		}
		else
		{
			leds[i] = four;
			j = -1;
		}
		j++;
	}
}
void everyThird(CRGB one, CRGB two, CRGB three)
{
	int j = 0;
	for (long i = 0; i < NUM_LEDS; i++)
	{

		if (j == 0)
		{
			leds[i] = one;
		}
		else if (j == 1)
		{
			leds[i] = two;
		}
		else
		{
			leds[i] = three;
			j = -1;
		}
		j++;
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
			color = leds[i - 1];
			while (color == leds[i - 1])
			{
				long rand = random(0, numColors - 1);
				color = colors[rand];
			}
		}
		leds[i] = color;
	}
}
void everyOther(CHSV one, CHSV two)
{
	for (long i = 0; i < NUM_LEDS; i++)
	{
		if (i % 2 == 0)
		{
			leds[i] = one;
		}
		else
		{
			leds[i] = two;
		}
	}
	show();
}

void showFixed(CRGB color)
{
	leds.fill_solid(color);
	show();
}

void showFixed(CHSV hsv)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = hsv;
	}
	show();
}

void showFixedHalf(CRGB color)
{
	for (long i = 0; i < NUM_LEDS; i++)
	{
		if (i % 2 == 0)
		{
			leds[i] = color;
		}
		else
		{
			leds[i] = BLACK;
		}
	}
	show();
}



#endif
