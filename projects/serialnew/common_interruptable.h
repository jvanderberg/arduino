#include <FastLED.h>
#include "Arduino.h"
#include "common.h"
double pot1 = 255.0;
double pot2 = 255.0;
double pot3 = 255.0;
double lastPot2 = 255.0;
double lastPot3 = 255.0;
boolean color_changed = false;

boolean delay_and_check(boolean (*check_io)(), long pause)
{
	if (pause < 50)
	{
		delay(pause);
		return check_io();
	}
	for (long i = 0; i < pause; i += 50)
	{
		delay(50);
		if (check_io())
			return true;
	}
	return false;
}

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
	delay_and_check(check_io, 100l);
}

boolean sparkle_decay(boolean (*check_io)(),CRGB color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = BLACK;
  }
  for (;;)
  {
    if (delay_and_check(check_io,20))
      return true;
    leds[random(0, NUM_LEDS - 1)] = color;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = leds[i].fadeToBlackBy(10);
    }
    show();
  }
}

boolean chaseThree(boolean (*check_io)(),long length, long time, long pause, CRGB one, CRGB two, CRGB three)
{
	long i;
	for (long k = 0; k < time; k += pause)
	{
		i = i + 1;
		for (long j = 0; j < NUM_LEDS; j++)
		{
			long mod = abs(((j + i) / (length)) % 3);
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
		if (delay_and_check(check_io, pause))
		{
			return true;
		}

	}
  return false;
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

boolean everyOtherAdjustable(boolean (*check_io)(), CHSV one, CHSV two)
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
			return true;
	}
}

boolean flashingEveryOtherAdjustable(boolean (*check_io)(), CHSV one, CHSV two)
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
			return true;
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
			return true;
	}
}

boolean sparkleDecayAdjustable(boolean (*check_io)(), CHSV color)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	while (!(*check_io)())
	{
		if (delay_and_check(check_io, 20l))
			return true;
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

boolean pulse(boolean (*check_io)(),CRGB color, long time, long pause)
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
		if (delay_and_check(check_io, pause))
			return true;
		brightness = brightness + fadeAmount;
		// reverse the direction of the fading at the ends of the fade:
		if (brightness == 0 || brightness == 255)
		{
			fadeAmount = -fadeAmount;
		}
		if (check_io())
			return true;
	}
  return false;
}

boolean switchColor(boolean (*check_io)(),long time, long pause, CRGB one, CRGB two)
{
	for (long i = 0; i < time; i += 2 * pause)
	{
		everyOther(one, two);
		show();
    if (delay_and_check(check_io, pause))
      return true;
    everyOther(two, one);
		show();
    if (delay_and_check(check_io, pause))
      return true;
	}
  return false;
}

boolean chase(boolean (*check_io)(),long length, long time, long pause, CRGB one, CRGB two)
{
	long i;
  Serial.println("chase");
  Serial.println(time);
  Serial.println(pause);

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
		if (delay_and_check(check_io, pause)) {
      Serial.println("got io");
			return true;
		}
	}
  return false;
}


boolean runningChase(boolean (*check_io)(), CHSV color)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	long index = 0l;
	while (!(*check_io)())
	{

		if (delay_and_check(check_io, 5l))
			return true;
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

boolean runningColor(boolean (*check_io)())
{
	CHSV color;
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = BLACK;
	}
	while (!(*check_io)())
	{
		if (delay_and_check(check_io, 5l))
			return true;
		color = CHSV(pot2, pot3, 255);
		leds[0] = color;
		for (int i = NUM_LEDS - 1; i > 0; i--)
		{
			leds[i] = leds[i - 1];
		}
		show();
	}
}

boolean flash(boolean (*check_io)(), long loops, CRGB color)
{
	for (long i = 0; i < loops; i++)
	{

		showFixed(color);
		if (delay_and_check(check_io,random(30, 100)))
			return true;
		showFixed(BLACK);
		if (delay_and_check(check_io,random(20, 100)))
			return true;
	}
	return false;
}


boolean flashHalf(boolean (*check_io)(),long loops, CRGB color)
{
	for (long i = 0; i < loops; i++)
	{

		showFixedHalf(color);
		if (delay_and_check(check_io,random(30, 100)))
			return true;
		showFixedHalf(BLACK);
		if (delay_and_check(check_io,random(20, 100)))
			return true;
	}
	return false;
}
