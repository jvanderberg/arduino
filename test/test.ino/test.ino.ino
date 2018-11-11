// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hlong: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include "FastLED.h"

#include <EEPROM.h>;
///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// How many leds are in the strip?
#define NUM_LEDS 150

// Data pin that led data will be written out over
#define DATA_PIN 2
long SPEED = 2000l;
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

// This is an array of leds.  One item for each led in your strip.
CRGBArray<NUM_LEDS> leds;
const CRGB WHITE = CRGB(255, 255, 255);
const CRGB PURPLE = CRGB(30, 0, 60);
const CRGB ORANGE = CRGB(80, 20, 0);
const CRGB BLACK = CRGB(0, 0, 0);
const CRGB LIGHT_BLUE = CRGB(50, 50, 200);
const CRGB PINK = CRGB(255, 30, 30);
int r = random(0, 255);
int g = random(0, 255);
int b = random(0, 255);
int pattern = 0;
boolean highSpeed = false;
int lastPattern = 0;
boolean lastHighSpeed = false;
CRGB randomColors2[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};

// This function sets up the ledsand tells the controller about them
void setup()
{
	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
	Serial.begin(115200); //set baud rate
	EEPROM.get(0, pattern);
	Serial.println("EEPROM Pattern:");
	Serial.println(pattern); //show the data
	EEPROM.get(sizeof(int), highSpeed);
	Serial.println("EEPROM HighSpeed:");
	Serial.println(highSpeed); //show the data
	lastHighSpeed = highSpeed;
	lastPattern = pattern;
}

boolean check_serial()
{
	while (Serial.available())
	{ //while there is data available on the serial monitor
		int value = char(Serial.read());
		Serial.println(value);
		pattern = value % (int)'a';
		Serial.println("Command:");
		Serial.println(pattern);
		SPEED = 1l;
		return true;
	}
	return false;
}

void pulse(CRGB color, long time, long pause)
{
	long fadeAmount = 1; // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
	long brightness = 0;
	for (long i = 0; i < time; i += pause)
	{
		for (long i = 0; i < NUM_LEDS; i++)
		{
			leds[i] = color;
			leds[i].fadeLightBy(brightness);
		}
		show();
		delay(pause); // This delay sets speed of the fade. I usually do from 5-75 but you can always go higher.
		brightness = brightness + fadeAmount;
		// reverse the direction of the fading at the ends of the fade:
		if (brightness == 0 || brightness == 255)
		{
			fadeAmount = -fadeAmount;
		}
		if (check_serial())
			break;
	}
}

void showFixed(CRGB color)
{
	leds.fill_solid(color);
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

void fadeall(long amount)
{
	for (long i = 0; i < NUM_LEDS; i++)
	{
		leds[i].nscale8(amount);
	}
}

void flash(long loops, CRGB color)
{
	for (long i = 0; i < loops; i++)
	{

		showFixed(color);
		delay(random(30, 100));
		showFixed(BLACK);
		delay(random(20, 100));
	}
}

void flashHalf(long loops, CRGB color)
{
	for (long i = 0; i < loops; i++)
	{

		showFixedHalf(color);
		delay(random(30, 100));
		showFixedHalf(BLACK);
		delay(random(20, 100));
	}
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

void halloween(long time, long pause)
{
	for (long i = 0; i < time; i += 2 * pause)
	{
		if (check_serial())
			break;
		everyOther(PURPLE, ORANGE);
		show();
		if (check_serial())
			break;
		delay(pause);
		everyOther(ORANGE, PURPLE);
		show();
		if (check_serial())
			break;
		delay(pause);
	}
}

void switchColor(long time, long pause, CRGB one, CRGB two)
{
	for (long i = 0; i < time; i += 2 * pause)
	{
		if (check_serial())
			break;
		everyOther(one, two);
		show();
		if (check_serial())
			break;
		delay(pause);
		if (check_serial())
			break;
		show();
		if (check_serial())
			break;
		delay(pause);
	}
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
		show();
		if (check_serial())
			break;
		delay(pause);
	}
}

void chaseThree(long length, long time, long pause, CRGB one, CRGB two, CRGB three)
{
	long i;
	for (long k = 0; k < time; k += pause)
	{
		i = i + 1;
		for (long j = 0; j < NUM_LEDS; j++)
		{
			long mod = ((j + i) / (3 * length)) % 3;
			if (mod == 0)
			{
				leds[j] = one;
			}
			else if (mod == 1)
			{
				leds[j] = two;
			}
			else
			{
				leds[j] = three;
			}
		}
		show();
		if (check_serial())
			break;
		delay(pause);
	}
}

void clear()
{
	for (int i = 0; i < 148; i++)
	{
		leds[i] = BLACK;
	}
	show();
}
void show()
{
	FastLED.show();
}
void halloweenShow()
{
	clear();

	if (check_serial())
		return;
	pulse(CRGB(0, 200, 0), SPEED * 4l, 20);
	if (check_serial())
		return;
	flash(15, CRGB(0, 200, 0));
	if (check_serial())
		return;
	pulse(PURPLE, SPEED * 4l, 20);
	if (check_serial())
		return;
	flash(15, PURPLE);
	if (check_serial())
		return;
	pulse(ORANGE, SPEED * 4l, 20);
	if (check_serial())
		return;
	flash(15, ORANGE);
	if (check_serial())
		return;
	chase(14, SPEED * 4l, 50, CRGB(0, 255, 0), BLACK);
	if (check_serial())
		return;
	flash(15, CRGB(0, 200, 0));
	if (check_serial())
		return;
	halloween(SPEED * 4l, 1000);
	if (check_serial())
		return;
	flash(15, PURPLE);
	if (check_serial())
		return;
	chase(10, SPEED * 4l, 50, ORANGE, PURPLE);
	if (check_serial())
		return;
	flashHalf(15, WHITE);
}

void thanksgivingShow()
{
	if (check_serial())
		return;
	everyThird(CRGB(255, 0, 0), CRGB(200, 100, 0), ORANGE);
	if (check_serial())
		return;
	show();
	delay(2000l);
	everyThird(CRGB(200, 100, 0), ORANGE, CRGB(255, 0, 0));
	if (check_serial())
		return;
	show();
	delay(2000l);
}

void valentines()
{
	if (check_serial())
		return;
	chase(14, SPEED * 4l, 50, CRGB(255, 0, 0), CRGB(255, 30, 30));
	showFixed(CRGB(255, 0, 0));
	for (int i = 0; i < 20; i++)
	{
		if (check_serial())
			return;
		delay(SPEED / 20);
	}
	showFixed(CRGB(255, 30, 30));
	for (int i = 0; i < 20; i++)
	{
		if (check_serial())
			return;
		delay(SPEED / 20);
	}
}

void fourth()
{
	chaseThree(5, SPEED * 4l, 100, CRGB(255, 0, 0), CRGB(200, 200, 200), CRGB(0, 0, 200));
	if (check_serial())
		return;
	everyThird(CRGB(255, 0, 0), CRGB(200, 200, 200), CRGB(0, 0, 200));
	if (check_serial())
		return;
	show();
	delay(SPEED);
	for (int i = 0; i < 20; i++)
	{
		showFixed(CRGB(255, 0, 0));
		if (check_serial())
			break;
		delay(2000);
		if (check_serial())
			break;
		showFixed(CRGB(200, 200, 200));
		delay(2000);
		if (check_serial())
			break;
		showFixed(CRGB(0, 0, 200));
		delay(2000);
		if (check_serial())
			break;
	}
}

void xmas()
{
	chase(10, SPEED * 4l, 100, CRGB(255, 0, 0), CRGB(0, 255, 0));
	if (check_serial())
		return;
	chase(10, SPEED * 4l, 100, CRGB(255, 0, 0), CRGB(200, 200, 200));
	if (check_serial())
		return;
	switchColor(SPEED * 4l, 1000, CRGB(0, 0, 255), CRGB(200, 200, 200));
	if (check_serial())
		return;
	CRGB randomColors[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};
	randomColor(randomColors, 5);
	if (check_serial())
		return;
}

void bears()
{
	chase(10, SPEED * 4l, 100, CRGB(0, 0, 200), ORANGE);
}
void st_pats()
{
	if (check_serial())
		return;
	showFixed(CRGB(0, 240, 0));
	if (check_serial())
		return;
	for (int i = 0; i < 20; i++)
	{
		if (check_serial())
			return;
		delay(SPEED / 20);
	}
	if (check_serial())
		return;
	chase(10, SPEED * 4l, 100, CRGB(0, 255, 0), CRGB(200, 200, 200));
}

void easter()
{
	everyFourth(PINK, LIGHT_BLUE, CRGB(230, 150, 0), CRGB(50, 255, 50));
	if (check_serial())
		return;
	show();
	delay(2000l);
	everyFourth(LIGHT_BLUE, CRGB(230, 150, 0), CRGB(50, 255, 50), PINK);
	if (check_serial())
		return;
	show();
	delay(2000l);
}
void show_color(CRGB color)
{
	showFixed(color);
	if (check_serial())
		return;
	for (int i = 0; i < 20; i++)
	{
		if (check_serial())
			return;
		delay(SPEED / 20);
	}
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{
	Serial.println("Pattern:");
	Serial.println(pattern);
	Serial.println("High Speed:");
	Serial.println(highSpeed);
	if (highSpeed)
	{
		SPEED = 2000l;
	}
	else
	{
		SPEED = 40000l;
	}
	if (lastPattern != pattern)
	{
		clear();
	}
	switch (pattern)
	{
	case 0:
		Serial.println("XMas");
		xmas();
		break;
	case 1:
		Serial.println("Fourth");
		fourth();
		break;
	case 2:
		Serial.println("Halloween");
		halloweenShow();
		break;
	case 3:
		Serial.println("Thanksgiving");
		thanksgivingShow();
		break;
	case 4:
		Serial.println("St Pats");
		st_pats();
		break;
	case 5:
		Serial.println("Valentines");
		valentines();
		break;
	case 6:
		Serial.println("Pink");
		show_color(PINK);
		break;
	case 7:
		Serial.println("Red");
		show_color(CRGB(255, 0, 0));
		break;
	case 8:
		Serial.println("White");
		show_color(CRGB(255, 255, 255));
		break;
	case 9:
		Serial.println("Blue");
		show_color(CRGB(0, 0, 255));
		break;
	case 10:
		Serial.println("Green");
		show_color(CRGB(0, 255, 0));
		break;
	case 11:
		Serial.println("Yellow");
		show_color(CRGB(255, 180, 0));
		break;
	case 12:
		Serial.println("Orange");
		show_color(ORANGE);
		break;
	case 13:
		Serial.println("Light Blue");
		show_color(LIGHT_BLUE);
		break;
	case 14:
		Serial.println("Purple");
		show_color(CRGB(100, 0, 255));
		break;
	case 15:
		Serial.println("Magenta");
		show_color(CRGB(255, 0, 255));
		break;
	case 16:
		Serial.println("Cyan");
		show_color(CRGB(0, 255, 255));
		break;
	case 17:
		Serial.println("Bears");
		bears();
		break;
	case 18:
		Serial.println("Easter");
		easter();
		break;
	case 19:
		Serial.println("Moving Rainbow");
		for (int hue = 0; hue < 255; hue++)
		{
			fill_rainbow(leds, NUM_LEDS, hue, 7);
			show();
			delay(10l);
			if (check_serial())
				return;
		}
		break;
	case 20:
		Serial.println("Solid Rainbow");
		fill_rainbow(leds, NUM_LEDS, 0, 7);
		show();
		delay(1000l);
		if (check_serial())
			return;
		break;
	case 21:
		Serial.println("Random Cycle");
		randomColor(randomColors2, 5);
		if (check_serial())
			return;
		show();
		delay(2000l);
		break;
	case 22:
		Serial.println("Resetting Random Color");
		r = random(0, 255);
		g = random(0, 255);
		b = random(0, 255);
		pattern = 23;
		break;
	case 23:
		Serial.println("Random Color");
		showFixed(CRGB(r, g, b));
		if (check_serial())
			return;
		delay(1000l);
		break;
	case 24:
		Serial.println("Crazy");
		flash(15, CRGB(255, 255, 255));
		if (check_serial())
			return;
		break;

	case 25:
		Serial.println("High Speed?");
		Serial.println(highSpeed);
		//Toggle speed
		if (highSpeed)
		{
			highSpeed = false;
			Serial.println("Regular speed");
		}
		else
		{
			highSpeed = true;
			Serial.println("High speed");
		}
		pattern = 0;
		break;
	default:
		pattern = 0;
	}
	if (lastPattern != pattern)
	{
		Serial.println("Saving pattern");
		EEPROM.put(0, pattern);
	}
	if (lastHighSpeed != highSpeed)
	{
		Serial.println("Saving highspeed");
		EEPROM.put(sizeof(int), highSpeed);
	}
	lastPattern = pattern;
	lastHighSpeed = highSpeed;
}
