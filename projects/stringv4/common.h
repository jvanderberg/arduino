#ifndef common_h
#define common_h
#include <FastLED.h>
#include "Arduino.h"
#define DEBUG 1
// How many leds are in the strip ?
#define NUM_LEDS 111

// Data pin that led data will be written out over
#define DATA_PIN 2
#define DELAY 20000l
// This is an array of leds.  One item for each led in your strip.
CRGBArray<NUM_LEDS> leds;
int pattern = 0;
int lastPattern = 0;
boolean off = 0;

const PROGMEM CRGB randomColors2[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};
const PROGMEM CRGB WHITE = CRGB(255, 255, 255);
const PROGMEM CRGB PURPLE = CRGB(30, 0, 60);
const PROGMEM CRGB ORANGE = CRGB(80, 20, 0);
const PROGMEM CRGB BLACK = CRGB(0, 0, 0);
const PROGMEM CRGB LIGHT_BLUE = CRGB(50, 50, 200);
const PROGMEM CRGB PINK = CRGB(255, 30, 30);

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

boolean flash(boolean (*check_io)(), long loops, CRGB color)
{
	for (long i = 0; i < loops; i++)
	{

		showFixed(color);
		delay(random(30, 100));
		showFixed(BLACK);
		delay(random(30, 100));
	}
	return false;
}

#endif
