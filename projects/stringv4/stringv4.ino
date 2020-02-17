// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hlong: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include "common.h"
#include <EEPROM.h>;
///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8
//#define DEBUG 1
#define LANES 4

// This is an array of leds.  One item for each led in your strip.
int r = random(0, 255);
int g = random(0, 255);
int b = random(0, 255);
int input = -1;
float newv[LANES];
float v[LANES];
float oldnewv[LANES];
float oldv[LANES];
unsigned long lastSample = millis();
unsigned long times = 0;
unsigned long lastTimes = 0;
unsigned long winTime = millis();
unsigned long lastButtonRead = millis();
unsigned long lastTrip[LANES];
unsigned int place = 1;
unsigned int places[LANES];
int button = 0;
int winner;
unsigned long lastHit;
const PROGMEM CRGB placeColors[] = {CRGB(0, 0, 0), CRGB(0, 255, 0), CRGB(200, 150, 0), CRGB(255, 0, 0), CRGB(0, 0, 255)};
unsigned long lastPrint = millis();
boolean done = true;
unsigned int debugLane = 0;
unsigned long lastTimer = millis();

#define BOTTOM_START 55
#define BOTTOM_END 106
#define TRACK4_START 0
#define TRACK4_END 12
#define TRACK3_START 13
#define TRACK3_END 25
#define TRACK2_START 26
#define TRACK2_END 38
#define TRACK1_START 39
#define TRACK1_END 50

#define INIT 0
#define READY 1
#define RACING 2
#define RESULTS 3
#define DEBUG 4
#define ACTION_BUTTON_PRESSED 0
#define ACTION_LANE_TRIP 1
#define ACTION_READY 2
#define ACTION_RESULTS_TIMEOUT 3
#define ACTION_TIMER 4

unsigned int currentState = INIT;
unsigned long startFinish;

// This function sets up the ledsand tells the controller about them
void setup()
{
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200); //set baud rate
	pinMode(3, INPUT_PULLUP);
	button = digitalRead(3);
	//Stabalize potentiometer readings
	show_basic();
	analogReference(DEFAULT);
}

void show_basic()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		if (i < BOTTOM_END && i >= BOTTOM_START)
		{
			leds[i] = CRGB(255, 255, 255);
		}
		else if (i >= TRACK4_START && i <= TRACK1_END)
		{
			leds[i] = CRGB(0, 50, 0);
		}

		else
		{
			leds[i] = CRGB(100, 0, 0);
		}
	}
	FastLED.show();
}

void show_level(int lane)
{
	boolean tripped = false;
	if (newv[lane] - v[lane] > 20)
	{
		tripped = true;
	}
	int width = TRACK1_END - (TRACK4_START + LANES);
	for (int i = 0; i < NUM_LEDS; i++)
	{

		int vl = (v[lane] / 1024.0) * width + LANES;
		int nvl = (newv[lane] / 1024.0) * width + LANES;
		if (i > TRACK4_START + LANES && i < TRACK1_END)
		{
			if (i == vl)
			{
				leds[i] = CRGB(255, 0, 0);
			}
			else if (i == nvl)
			{
				leds[i] = CRGB(0, 255, 0);
			}
			else
			{
				leds[i] = CRGB(50, 50, 50);
			}
		}
		else if (i <= TRACK4_START + LANES)
		{
			if (i <= TRACK4_START + lane)
			{
				leds[i] = CRGB(0, 200, 0);
			}
			else
			{
				leds[i] = CRGB(50, 50, 50);
			}
		}
		else if (i >= TRACK1_END && i <= TRACK1_END + 5)
		{
			if (tripped)
			{
				leds[i] = CRGB(0, 0, 200);
			}
			else
			{
				leds[i] = CRGB(255, 0, 0);
			}
		}
		else
		{
			leds[i] = CRGB(255, 255, 255);
		}
	}
	FastLED.show();
}

void show_winners()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		if (i >= TRACK1_START && i <= TRACK1_END)
		{
			leds[i] = placeColors[places[0]];
		}
		else if (i >= TRACK2_START && i <= TRACK2_END)
		{
			leds[i] = placeColors[places[1]];
		}
		else if (i >= TRACK3_START && i <= TRACK3_END)
		{
			leds[i] = placeColors[places[2]];
		}
		else if (i >= TRACK4_START && i <= TRACK4_END)
		{
			leds[i] = placeColors[places[3]];
		}
	}
	FastLED.show();
}

void chase(long length, long time, long pause, CRGB one, CRGB two)
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
				leds[j] = one;
			}
			else
			{
				leds[j] = two;
			}
		}
		FastLED.show();

		delay(pause);
	}
}
boolean handleLaneTrip(unsigned int lane)
{
	boolean done = false;
	unsigned long now = millis();
	lastHit = now;
	lastTrip[lane] = now;
	if (places[lane] == 0)
	{
		places[lane] = place;
		place++;
	}
	if (place > LANES)
	{
		done = true;
	}
	return done;
}

void dispatch(unsigned int action, unsigned long payload)
{
	// Serial.println("==============");
	// Serial.print("Action: ");
	// Serial.println(action);

	// Serial.print("payload: ");
	// Serial.println(payload);
	// Serial.print("oldstate: ");
	// Serial.println(currentState);
	if (action == ACTION_TIMER)
	{
		Serial.println(times - lastTimes);
		times = lastTimes;
	}

	switch (currentState)
	{
	case INIT:
		currentState = READY;
		lastHit = millis();
		Serial.println("Resetting");
		place = 1;
		for (int i = 0; i < LANES; i++)
		{
			places[i] = 0;
		}
		break;
	case READY:
		if (action == ACTION_BUTTON_PRESSED)
		{
			currentState = DEBUG;
		}
		if (action == ACTION_LANE_TRIP)
		{
			startFinish = millis();
			handleLaneTrip(payload);
			currentState = RACING;
		}
		break;
	case RACING:
		if (action == ACTION_TIMER && payload - startFinish > 5000)
		{
			//Race timeout
			currentState = RESULTS;
			winTime = millis();
		}
		if (action == ACTION_LANE_TRIP)
		{
			if (handleLaneTrip(payload))
			{
				currentState = RESULTS;
				winTime = millis();
			}
		}

		break;
	case RESULTS:
		if (action == ACTION_TIMER && payload - winTime > 10000)
		{
			currentState = INIT;
		}

		break;
	case DEBUG:
		if (action == ACTION_BUTTON_PRESSED)
		{
			debugLane++;
			Serial.print("Debug Lane: ");
			Serial.println(debugLane);
			if (debugLane == 4)
			{
				debugLane = 0;
				currentState = INIT;
			}
		}
	}
	// Serial.print("newstate: ");
	// Serial.println(currentState);
}

void render(unsigned int state)
{
	switch (currentState)
	{
	case INIT:
		chase(7, 5000, 50, CRGB(200, 0, 0), CRGB(0, 255, 0));
		show_basic();
		for (int lane = 0; lane < LANES; lane++)
		{
			newv[lane] = (float)analogRead(lane);
			v[lane] = newv[lane];
		}
		break;
	case READY:
		break;
	case RACING:
		//No display, as this would just slow down the timing
		break;
	case RESULTS:
		show_winners();
		break;
	case DEBUG:
		show_level(debugLane);
		break;
	}
}
// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{

	int value = digitalRead(3);
	if (button != value)
	{
		button = value;
		if (millis() - lastButtonRead > 500)
		{
			dispatch(ACTION_BUTTON_PRESSED, 0);
			lastButtonRead = millis();
		}
	}

	//delay(20);
	times++;
	if (millis() - lastTimer > 1000)
	{
		dispatch(ACTION_TIMER, millis());
		lastTimer = millis();
	}
	for (int lane = 0; lane < LANES; lane++)
	{
		newv[lane] = 0.8 * newv[lane] + 0.2 * (float)analogRead(lane);
		v[lane] = v[lane] * 0.95 + 0.05 * newv[lane];
	}
	for (int lane = 0; lane < LANES; lane++)
	{
		unsigned long now = millis();
		if (((now - lastTrip[lane]) > 1000) && newv[lane] - v[lane] > 40)
		{
			dispatch(ACTION_LANE_TRIP, lane);
		}
	}
	// if (millis() - lastSample > 20000)
	// {
	// 	lastSample = millis();
	// 	for (int lane = 0; lane < LANES; lane++)
	// 	{
	// 		oldnewv[lane] = newv[lane];
	// 		oldv[lane] = v[lane];
	// 	}
	// }

	// if (millis() - lastPrint > 1000)
	// {
	// 	lastPrint = millis();
	// 	for (int lane = 0; lane < LANES; lane++)
	// 	{
	// 		Serial.print(lane);
	// 		Serial.print(": ");
	// 		Serial.println(newv[lane]);
	// 		Serial.print(" - ");
	// 		Serial.println(v[lane]);
	// 	}
	// }
	render(currentState);
}
// if (millis() - lastHit > 3000 && place > 1 && !done)
// {
// 	done = true;
// }

// if (done && mode == 0)
// {
// 	show_winners();
// }
// if (millis() - lastHit > 10000 && mode == 0)
// {
// 	show_basic();
// 	done = false;
// 	lastHit = millis();
// 	Serial.println("Resetting");
// 	winner = -1;
// 	place = 1;
// 	for (int i = 0; i < LANES; i++)
// 	{
// 		places[i] = 0;
// 	}
// }
//}
