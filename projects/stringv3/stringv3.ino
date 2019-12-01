// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hlong: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>
#include "MyLED2.h"
#include <EEPROM.h>;
///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// How many leds are in the strip?
#define NUM_LEDS 400

// Data pin that led data will be written out over
#define DATA_PIN 2
long SPEED = 2000l;
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8
//#define DEBUG 1

// This is an array of leds.  One item for each led in your strip.
CRGBArray<NUM_LEDS> leds;
int r = random(0, 255);
int g = random(0, 255);
int b = random(0, 255);
int pattern = 0;
int input = -1;
int off = 0;
double pot1 = 255.0;
double pot2 = 255.0;
double pot3 = 255.0;
double lastPot2 = 255.0;
double lastPot3 = 255.0;
boolean color_changed = false;
int lastPattern = 0;
CRGB randomColors2[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};

template <int SIZE>
class LED : public MyLED<SIZE>
{

  public:
	LED(long speed, CRGBArray<SIZE> leds) : MyLED<SIZE>(speed, leds)
	{
	}
	void xmas();
	boolean check_io();
};

template <int SIZE>
void LED<SIZE>::xmas()
{
	MyLED<SIZE>::xmas();
}

// This function sets up the ledsand tells the controller about them
void setup()
{
	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
	Serial.begin(115200); //set baud rate
	EEPROM.get(0, pattern);
	debug("EEPROM Pattern:");
	debug(pattern); //show the data
	lastPattern = pattern;
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	//Stabalize potentiometer readings
	for (int i = 0; i < 100; i++)
	{
		read_pots();
		Serial.println("Pot2:");
		Serial.println(pot2);
		Serial.println("Pot1:");
		Serial.println(pot1);
	}
	lastPot2 = pot2;
	lastPot3 = pot3;
}

PROGMEM const LED<NUM_LEDS> led{SPEED, leds};

/**
 * Read the potentiometers
 */
void read_pots()
{
	double newpot1 = ((double)analogRead(0) / 1024.0) * 255;
	pot1 = (int)(0.25 * newpot1 + 0.75 * pot1);
	double newpot2 = ((double)analogRead(1) / 1024.0) * 255;
	pot2 = (int)(0.25 * newpot2 + 0.75 * pot2);
	double newpot3 = ((double)analogRead(2) / 1024.0) * 255;
	pot3 = (int)(0.25 * newpot3 + 0.75 * pot3);
}

template <int SIZE>
boolean LED<SIZE>::check_io()
{
	int value = digitalRead(4);
	off = digitalRead(5);
	debug("Off");
	debug(off);
	debug(value);
	debug(input);
	debug('-----');
	debug("Pattern:");
	debug(pattern);
	read_pots();
	if (abs(pot2 - lastPot2) > 10 || abs(pot3 - lastPot3) > 10)
	{
		color_changed = true;
	}

	debug("Pot0:");
	debug(pot1);
	debug("Pot1:");
	debug(pot2);

	//	setBrightness();
	if (input > -1 && value != input)
	{
		input = value;
		pattern = ((pattern + 1) % 26);
		Serial.println("Command:");
		Serial.println(pattern);
		lastPot2 = pot2;
		lastPot3 = pot3;
		color_changed = false;
		return true;
	}
	else
	{
		input = value;
	}

	return false;
}

void debug(const String &s)
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

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{
	CHSV one;
	CHSV two;
	debug("Pattern   1:");
	debug(pattern);
	//setBrightness();

	if (lastPattern != pattern)
	{
		//clear();
	}
	switch (pattern)
	{
	case 0:
		debug("XMas");
		led.xmas();
		break;

	default:
		pattern = 0;
	}
	if (lastPattern != pattern)
	{
		debug("Saving pattern");
		EEPROM.put(0, pattern);
	}
	lastPattern = pattern;
}
