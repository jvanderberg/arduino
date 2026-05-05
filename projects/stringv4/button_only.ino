// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hlong: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include "common.h"
#include <EEPROM.h>;

int r = random(0, 255);
int g = random(0, 255);
int b = random(0, 255);
int input = -1;
int brightness = 255;
int hue = 255;

// This function sets up the ledsand tells the controller about them
void setup()
{
	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
	Serial.begin(115200); //set baud rate
	EEPROM.get(0, pattern);
	Serial.println("EEPROM Pattern:");
	Serial.println(pattern); //show the data
	lastPattern = pattern;
	pinMode(4, INPUT_PULLUP);
}

boolean check_io_no_brightness()
{
	return check_io(false);
}
boolean check_io()
{
	return check_io(true);
}
boolean check_io(boolean set_brightness)
{
	//Serial.println("check_io");
	int value = digitalRead(4);
	debug(value);
	debug(input);
	debug('-----');
	debug("Pattern:");
	debug(pattern);
	if (set_brightness)
	{
		FastLED.setBrightness(brightness);
		show();
	}
	if (input > -1 && value != input)
	{
		input = value;
		pattern = ((pattern + 1) % 100);
		Serial.println("Command:");
		Serial.println(pattern);
		return true;
	}
	else
	{
		input = value;
	}
	while (Serial.available())
	{ //while there is data available on the serial monitor
		int value = char(Serial.read());
		if (value == 32)
		{ //Simulate button click
			pattern = ((pattern + 1) % 100);
			Serial.println("Command:");
			Serial.println(pattern);
			return true;
		}
		Serial.println(value);
		pattern = value % (int)'a';
		Serial.println("Command:");
		Serial.println(pattern);
		return true;
	}

	return false;
}

boolean check_io_brightness()
{
	Serial.println("check_io_brightness");
	int value = digitalRead(4);
	debug(value);
	debug(input);
	debug('-----');
	debug("Pattern:");
	debug(pattern);

	if (input > -1 && value != input)
	{
		input = value;
		brightness = ((brightness + 10) % 255);
		Serial.println("Brightness:");
		Serial.println(brightness);
		FastLED.setBrightness(brightness);
		show();
		return true;
	}
	else
	{
		input = value;
	}
	while (Serial.available())
	{				   //while there is data available on the serial monitor
		Serial.read(); //Don't care about the value
		brightness = ((brightness + 10) % 255);
		Serial.println("Brightness:");
		Serial.println(brightness);
		FastLED.setBrightness(brightness);
		show();
		return true;
	}
	return false;
}

boolean check_io_hue()
{
	Serial.println("check_io_hue");
	int value = digitalRead(4);
	debug(value);
	debug(input);
	debug('-----');
	debug("Pattern:");
	debug(pattern);

	if (input > -1 && value != input)
	{
		input = value;
		hue = ((hue + 5) % 255);
		Serial.println("Hue:");
		Serial.println(hue);
		showFixed(CHSV(hue, 255, 255));
		FastLED.setBrightness(brightness);
		show();
		return true;
	}
	else
	{
		input = value;
	}

	while (Serial.available())
	{ //while there is data available on the serial monitor

		Serial.read(); //Don't care about the value

		input = value;
		hue = ((hue + 5) % 255);
		Serial.println("Hue:");
		Serial.println(hue);
		showFixed(CHSV(hue, 255, 255));
		FastLED.setBrightness(brightness);
		show();
		return true;
	}
	return false;
}
void flash(int times, int pause)
{
	for (int i = 0; i < times; i++)
	{
		FastLED.setBrightness(brightness);
		show();
		delay(pause);
		FastLED.setBrightness(0);
		show();
		delay(pause);
	}
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{
	//Serial.println("loop");
	debug("Pattern   1:");
	//Serial.println(pattern);

	if (lastPattern != pattern)
	{
		clear();
	}
	switch (pattern)
	{
	case 0:
		Serial.println(F("XMas"));
		xmas(check_io);
		break;
	case 1:
		Serial.println(F("Red/White"));
		everyOther(CHSV(255, 0, 255), CHSV(255, 255, 255));
		delay_and_check(check_io, 50);
		break;
	case 2:
		debug(F("White/Blue"));
		everyOther(CHSV(255, 0, 255), CHSV(160, 255, 255));
		delay_and_check(check_io, 50);
		break;
	case 3:
		debug(F("Red/Green"));
		everyOther(CHSV(255, 255, 255), CHSV(100, 255, 255));
		delay_and_check(check_io, 50);
		break;
	case 4:
		debug("Pink");
		showFixed(CHSV(255, 150, 255));
		delay_and_check(check_io, 50);
		break;
	case 5:
		debug("Red");
		showFixed(CHSV(255, 255, 255));
		delay_and_check(check_io, 50);
		break;
	case 6:
		debug("White");
		showFixed(CHSV(255, 0, 255));
		delay_and_check(check_io, 50);
		break;
	case 7:
		debug("Blue");
		showFixed(CHSV(160, 255, 255));
		delay_and_check(check_io, 50);
		break;
	case 8:
		debug("Green");
		showFixed(CHSV(100, 255, 255));
		delay_and_check(check_io, 50);
		break;
	case 9:
		debug("Light Blue");
		showFixed(CHSV(160, 150, 255));
		delay_and_check(check_io, 50);
		break;
	case 10:
		debug("Random Cycle");
		randomColor(randomColors2, 5);
		delay_and_check(check_io, 2000l);
		break;

	case 11:
		debug("Yellow White");
		showFixed(CHSV(34, 178, 255));
		delay_and_check(check_io, 50);
		break;
	case 12:
		debug("White/Blue alternate");
		flashingEveryOther(check_io, CHSV(255, 0, 255), CHSV(160, 255, 255));
		break;
	case 13:
		debug("Red/white alternate");
		flashingEveryOther(check_io, CHSV(255, 0, 255), CHSV(255, 255, 255));
		break;
	case 14:
		debug("Blue white");
		showFixed(CHSV(160, 80, 255));
		delay_and_check(check_io, 50);
		break;
	case 15:
		debug("Sparkle Decay");
		sparkleDecay(check_io, CHSV(255, 0, 255));
		break;
	case 16:
		debug("Sparkle Decay Adjustable");
		sparkleDecay(check_io, CHSV(hue, 255, 255));
		break;
	case 17:
		debug("Thanksgiving");
		thanksgivingShow(check_io);
		break;
	case 18:
		debug("Random sparkle/decay");
		randomSparkleDecay(check_io);
		break;
	case 19:
		debug("Fourth");
		fourth(check_io);
		break;
	case 20:
		debug("Halloween");
		halloweenShow(check_io_no_brightness);
		break;
	case 21:
		debug("Valentines");
		valentines(check_io);
		break;
	case 22:
		debug("St Pats");
		st_pats(check_io);
		break;
	case 23:
		debug("Bears");
		bears(check_io);
		break;
	case 24:
		debug("Easter");
		easter(check_io);
		break;
	case 25:
		debug("Set Brightness");
		if (pulse(check_io_no_brightness, CHSV(hue, 255, 255), 5, 1400, 20))
		{
			break;
		}
		showFixed(CHSV(hue, 255, 255));
		flash(6, 50);

		FastLED.setBrightness(brightness);
		show();
		for (int i = 0; i < 1000; i++)
		{
			if (check_io_brightness())
			{
				i = 0;
			}
			delay(10);
		}
		flash(6, 50);

		while (!delay_and_check(check_io, 1000))
		{
		}
		break;

	case 26:
		debug("Set Hue");
		for (int i = 0; i < 255 * 3; i += 3)
		{
			showFixed(CHSV(i % 255, 255, 255));
			if (delay_and_check(check_io, 10))
			{
				return;
			}
		}
		showFixed(CHSV(hue, 255, 255));
		flash(6, 50);
		FastLED.setBrightness(brightness);
		show();
		for (int i = 0; i < 1000; i++)
		{
			if (check_io_hue())
			{
				i = 0;
			}
			delay(10);
		}
		flash(6, 50);

		while (!delay_and_check(check_io, 1000))
		{
		}
		break;

	case 27:
		Serial.println("Moving Rainbow");
		for (int hue = 0; hue < 255; hue++)
		{
			fill_rainbow(leds, NUM_LEDS, hue, 7);
			show();
			if (delay_and_check(check_io, 10))
			{
				return;
			}
		}
		break;

	default:
		pattern = 0;
	}
	if (lastPattern != pattern)
	{
		Serial.println("Saving pattern");
		EEPROM.put(0, pattern);
	}
	lastPattern = pattern;
}
