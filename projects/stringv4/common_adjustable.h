#include <FastLED.h>
#include "Arduino.h"
#include "common.h"
double pot1 = 255.0;
double pot2 = 255.0;
double pot3 = 255.0;
double lastPot2 = 255.0;
double lastPot3 = 255.0;
boolean color_changed = false;

void showColorAdjustable(boolean (*check_io)(), CHSV color)
{
	if (!color_changed)
	{
		showFixed(color);
	}
	else
	{
		CHSV hsv = CHSV(pot2, pot3, 255);
		showFixed(hsv);
	}
	delay_and_check(check_io, 100);
}

boolean chaseUserSet(boolean (*check_io)(), long length, long time, long pause)
{
	long i;

	for (long k = 0; k < time; k += pause)
	{
		CHSV one;
		one.hue = pot3;
		one.val = 255;
		one.sat = 255;
		CHSV two;
		two.hue = pot2;
		two.val = 255;
		two.sat = 255;
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

		if (delay_and_check(check_io, pause))
		{
			return true;
		}
	}
}

void everyOtherAdjustable(boolean (*check_io)(), CHSV one, CHSV two)
{

	for (int i = 1; i < 20; i++)
	{
		if (color_changed)
		{
			one = CHSV(pot2, 255, 255);
			two = CHSV(pot3, 255, 255);
		}
		everyOther(one, two);
		show();
		if (delay_and_check(check_io, 100l))
			return;
	}
}

void flashingEveryOtherAdjustable(boolean (*check_io)(), CHSV one, CHSV two)
{

	for (int i = 1; i < 20; i++)
	{
		if (color_changed)
		{
			one = CHSV(pot2, 255, 255);
			two = CHSV(pot3, 255, 255);
		}
		everyOther(one, two);
		show();
		if (delay_and_check(check_io, 100l))
			return;
	}
	for (int i = 1; i < 20; i++)
	{
		if (color_changed)
		{
			one = CHSV(pot2, 255, 255);
			two = CHSV(pot3, 255, 255);
		}
		everyOther(two, one);
		show();
		if (delay_and_check(check_io, 100l))
			return;
	}
}

void sparkleDecayAdjustable(boolean (*check_io)(), CHSV color)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	while (!(*check_io)())
	{
		if (delay_and_check(check_io, 20l))
			return;
		if (color_changed)
		{
			color = CHSV(pot2, pot3, 255);
		}
		leds[random(0, NUM_LEDS - 1)] = color;
		leds[random(0, NUM_LEDS - 1)] = color;
		leds[random(0, NUM_LEDS - 1)] = color;
		for (int i = 0; i < NUM_LEDS; i++)
		{
			if (leds[i].getLuma() > 30)
			{
				leds[i] = leds[i].fadeToBlackBy(10);
			}
		}
		show();
	}
}

void showhue(boolean (*check_io)())
{
	CHSV hsv;
	hsv.hue = pot2;
	hsv.val = 255;
	hsv.sat = pot3;
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = hsv;
	}
	show();
	delay_and_check(check_io, 100);
}

void runningChase(boolean (*check_io)(), CHSV color)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	long index = 0l;
	while (!(*check_io)())
	{

		if (delay_and_check(check_io, 5l))
			return;
		if (color_changed)
		{
			color = CHSV(pot2, pot3, 255);
		}
		color = CHSV(color.hue, 2 * abs(127 - (index * 5) % 255), 255);
		leds[0] = color;
		for (int i = NUM_LEDS - 1; i > 0; i--)
		{
			leds[i] = leds[i - 1];
		}
		index++;
		show();
	}
}
void runningColor(boolean (*check_io)())
{
	CHSV color;
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	while (!(*check_io)())
	{
		if (delay_and_check(check_io, 5l))
			return;
		color = CHSV(pot2, pot3, 255);
		leds[0] = color;
		for (int i = NUM_LEDS - 1; i > 0; i--)
		{
			leds[i] = leds[i - 1];
		}
		show();
	}
}
