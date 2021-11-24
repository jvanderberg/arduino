// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hlong: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#define NUM_LEDS 150
// This is an array of leds.  One item for each led in your strip.
#include "FastLED.h"
CRGBArray<NUM_LEDS> leds;
CRGB randomColors2[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};
const CRGB WHITE = CRGB(255, 255, 255);
const CRGB PURPLE = CRGB(30, 0, 60);
const CRGB ORANGE = CRGB(80, 20, 0);
const CRGB BLACK = CRGB(0, 0, 0);
const CRGB LIGHT_BLUE = CRGB(50, 50, 200);
const CRGB PINK = CRGB(255, 30, 30);
#include "common_interruptable.h"
#include <EEPROM.h>;
///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// How many leds are in the strip?


// Data pin that led data will be written out over
#define DATA_PIN 2
long SPEED = 200l;
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

int r = random(0, 255);
int g = random(0, 255);
int b = random(0, 255);
int pattern = 0;
boolean highSpeed = false;
int lastPattern = 0;
boolean lastHighSpeed = false;

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

boolean check_io()
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

boolean halloween(long time, long pause)
{
  for (long i = 0; i < time; i += 2 * pause)
  {
    everyOther(PURPLE, ORANGE);
    show();
    if (delay_and_check(check_io, pause))
      return true;
    everyOther(ORANGE, PURPLE);
    show();
    if (delay_and_check(check_io, pause))
      return true;
  }
  return false;
}

void halloweenShow()
{
  clear();

  if (pulse(check_io, CRGB(0, 200, 0), SPEED * 4l, 20))
    return;
  flash(check_io, 15l, CRGB(0, 200, 0));
  if (pulse(check_io, PURPLE, SPEED * 4l, 20))
    return;
  flash(check_io, 15l, PURPLE);
  if (pulse(check_io, ORANGE, SPEED * 4l, 20))
    return;

  flash(check_io, 15l, ORANGE);

  if (chase(check_io, 14, SPEED * 4l, 50, CRGB(0, 255, 0), BLACK))
    return;

  flash(check_io, 15l, CRGB(0, 200, 0));

  if (halloween(SPEED * 4l, 1000))
    return;

  flash(check_io, 15l, PURPLE);
  if (chase(check_io, 10, SPEED * 4l, 50, ORANGE, PURPLE))
    return;
  flashHalf(check_io, 15l, WHITE);
}

boolean thanksgivingShow()
{
  everyThird(CRGB(255, 0, 0), CRGB(200, 100, 0), ORANGE);

  show();
  if (delay_and_check(check_io, 2000l))
    return true;

  everyThird(CRGB(200, 100, 0), ORANGE, CRGB(255, 0, 0));

  show();
  if (delay_and_check(check_io, 2000l))
    return true;
}

boolean valentines()
{

  if (chase(check_io, 10, SPEED, 100, CRGB(255, 0, 0), CRGB(255, 30, 30)))
    return true;

  for (int i =0; i<20; i++) {
    showFixed(CRGB(255, 0, 0));
  
    if (delay_and_check(check_io, SPEED/20))
      return true;
    showFixed(CRGB(255, 30, 30));
  
    if (delay_and_check(check_io, SPEED/20))
      return true;
  }
}

boolean fourth()
{
  if (chaseThree(check_io, 10, SPEED * 4l, 100, CRGB(255, 0, 0), CRGB(200, 200, 200), CRGB(0, 0, 255)))
    return true;

  for (int i=0; i< 10; i++) {
    everyThird(CRGB(255, 0, 0), CRGB(200, 200, 200), CRGB(0, 0, 200));
    show();
    
    if (delay_and_check(check_io, SPEED/20))
      return true;
  
    everyThird( CRGB(200, 200, 200), CRGB(0, 0, 200), CRGB(255, 0, 0));
    show();
    
    if (delay_and_check(check_io, SPEED/20))
      return true;
 
  }
  
 
  showFixed(CRGB(255, 0, 0));
  if (delay_and_check(check_io, SPEED))
    return true;
  showFixed(CRGB(200, 200, 200));
  if (delay_and_check(check_io, SPEED))
    return true;
  showFixed(CRGB(0, 0, 200));
  if (delay_and_check(check_io, SPEED))
    return true;
}

boolean xmas()
{
  if (chase(check_io, 10, SPEED, 100, CRGB(255, 0, 0), CRGB(0, 255, 0))) {
    return true;
  }

  if (chase(check_io, 10, SPEED, 100, CRGB(255, 0, 0), CRGB(200, 200, 200)))
      return true;

  if (switchColor(check_io, SPEED, 1000, CRGB(0, 0, 255), CRGB(200, 200, 200)))
    return true;

  CRGB randomColors[] = {CRGB(255, 0, 0), CRGB(200, 150, 0), CRGB(0, 255, 0), CRGB(255, 0, 255), CRGB(0, 255, 255)};
  randomColor(randomColors, 5);
  if (delay_and_check(check_io, SPEED))
    return true;
}

boolean bears()
{
  return chase(check_io, 10, SPEED * 4l, 100, CRGB(0, 0, 200), ORANGE);
}

boolean st_pats()
{

  showFixed(CRGB(0, 240, 0));

  if (delay_and_check(check_io, SPEED))
    return true;
  return chase(check_io, 10, SPEED, 100, CRGB(0, 255, 0), CRGB(200, 200, 200));
}

boolean easter()
{
  everyFourth(PINK, LIGHT_BLUE, CRGB(230, 150, 0), CRGB(50, 255, 50));
  show();
  if (delay_and_check(check_io, 2000l))
    return true;
  everyFourth(LIGHT_BLUE, CRGB(230, 150, 0), CRGB(50, 255, 50), PINK);
  show();
  if (delay_and_check(check_io, 2000l))
    return true;
}

void show_color(boolean (*check_io)(), CRGB color)
{
  showFixed(color);
  delay_and_check(check_io, SPEED);
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
  //SPEED = 2000l;
  SPEED = 40000l;

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
      show_color(check_io, PINK);
      break;
    case 7:
      Serial.println("Red");
      show_color(check_io, CRGB(255, 0, 0));
      break;
    case 8:
      Serial.println("White");
      show_color(check_io, CRGB(255, 255, 255));
      break;
    case 9:
      Serial.println("Blue");
      show_color(check_io, CRGB(0, 0, 255));
      break;
    case 10:
      Serial.println("Green");
      show_color(check_io, CRGB(0, 255, 0));
      break;
    case 11:
      Serial.println("Yellow");
      show_color(check_io, CRGB(255, 180, 0));
      break;
    case 12:
      Serial.println("Orange");
      show_color(check_io, ORANGE);
      break;
    case 13:
      Serial.println("Light Blue");
      show_color(check_io, LIGHT_BLUE);
      break;
    case 14:
      Serial.println("Purple");
      show_color(check_io, CRGB(100, 0, 255));
      break;
    case 15:
      Serial.println("Magenta");
      show_color(check_io, CRGB(255, 0, 255));
      break;
    case 16:
      Serial.println("Cyan");
      show_color(check_io, CRGB(0, 255, 255));
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
        if (delay_and_check(check_io, 10l))
          return;
      }
      break;
    case 20:
      Serial.println("Solid Rainbow");
      fill_rainbow(leds, NUM_LEDS, 0, 7);
      show();
      if (delay_and_check(check_io, 1000l))
        return;
      break;
    case 21:
      Serial.println("Random Cycle");
      randomColor(randomColors2, 5);
      if (delay_and_check(check_io, 1000l))
        return;
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
      if (delay_and_check(check_io, 1000l))
        return;
      break;
    case 24:
      Serial.println("Sparkle snow");
      sparkle_decay(check_io, CRGB(255,255,255));
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
