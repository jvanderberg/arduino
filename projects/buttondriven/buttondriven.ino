#include "LEDAnimator.h"
#include <EEPROM.h>

// Hardware configuration
#define NUM_LEDS 300
#define DATA_PIN 2
#define BUTTON_PIN 7

// #define DEBUG 1

// Timings preserved from the original SPEED-based code
// (SPEED was effectively 40000ms; SPEED*4 was the long animation budget)
#define SHORT_DELAY    2000l
#define LONG_DELAY     40000l
#define ANIMATION_TIME 160000l

// LED array (Color is layout-compatible with CRGB)
Color leds[NUM_LEDS];

// Application state
int pattern = 0;
int lastPattern = -1;
int buttonState = -1;
int randR = 0, randG = 0, randB = 0;

// Color palette for random patterns
Color randomPalette[] = {Color(255, 0, 0), Color(200, 150, 0), Color(0, 255, 0), Color(255, 0, 255), Color(0, 255, 255)};

// Forward declarations
void debug(const String &s);
void debug(int i);

// === Input Checking ===
bool checkInput();

// Create the animator with our abort callback
LEDAnimator animator(leds, NUM_LEDS, checkInput);

// === Composite Patterns ===

void show_color(CRGB color) {
    animator.fill(color);
    animator.delayAndCheck(LONG_DELAY);
}

void halloweenShow() {
    animator.clear();
    if (animator.pulse(CRGB(0, 200, 0), ANIMATION_TIME)) return;
    if (animator.flash(15, CRGB(0, 200, 0))) return;
    if (animator.pulse(LED_PURPLE, ANIMATION_TIME)) return;
    if (animator.flash(15, LED_PURPLE)) return;
    if (animator.pulse(LED_ORANGE, ANIMATION_TIME)) return;
    if (animator.flash(15, LED_ORANGE)) return;
    if (animator.chase(14, CRGB(0, 255, 0), CRGB::Black, ANIMATION_TIME)) return;
    if (animator.flash(15, CRGB(0, 200, 0))) return;
    if (animator.alternate(LED_PURPLE, LED_ORANGE, ANIMATION_TIME, 1000)) return;
    if (animator.flash(15, LED_PURPLE)) return;
    if (animator.chase(10, LED_ORANGE, LED_PURPLE, ANIMATION_TIME)) return;
    animator.flashHalf(15, LED_WHITE);
}

void thanksgivingShow() {
    animator.everyThird(CRGB(255, 0, 0), CRGB(200, 100, 0), LED_ORANGE);
    animator.show();
    if (animator.delayAndCheck(2000)) return;
    animator.everyThird(CRGB(200, 100, 0), LED_ORANGE, CRGB(255, 0, 0));
    animator.show();
    animator.delayAndCheck(2000);
}

void valentines() {
    if (animator.chase(14, CRGB(255, 0, 0), CRGB(255, 30, 30), 20000l)) return;
    animator.fill(CRGB(255, 0, 0));
    if (animator.delayAndCheck(20000l)) return;
    animator.fill(CRGB(255, 30, 30));
    animator.delayAndCheck(20000l);
}

void fourth() {
    if (animator.chaseThree(5, CRGB(255, 0, 0), CRGB(200, 200, 200), CRGB(0, 0, 200), ANIMATION_TIME)) return;
    animator.everyThird(CRGB(255, 0, 0), CRGB(200, 200, 200), CRGB(0, 0, 200));
    animator.show();
    if (animator.delayAndCheck(LONG_DELAY)) return;
    for (int i = 0; i < 20; i++) {
        animator.fill(CRGB(255, 0, 0));
        if (animator.delayAndCheck(2000)) return;
        animator.fill(CRGB(200, 200, 200));
        if (animator.delayAndCheck(2000)) return;
        animator.fill(CRGB(0, 0, 200));
        if (animator.delayAndCheck(2000)) return;
    }
}

void xmas() {
    if (animator.chase(10, CRGB(255, 0, 0), CRGB(0, 255, 0), ANIMATION_TIME, 100)) return;
    if (animator.chase(10, CRGB(255, 0, 0), CRGB(200, 200, 200), ANIMATION_TIME, 100)) return;
    if (animator.alternate(CRGB(0, 0, 255), CRGB(200, 200, 200), ANIMATION_TIME, 1000)) return;
    animator.randomFill(randomPalette, 5);
    animator.delayAndCheck(2000);
}

void bears() {
    animator.chase(10, CRGB(0, 0, 200), LED_ORANGE, ANIMATION_TIME, 100);
}

void st_pats() {
    animator.fill(CRGB(0, 240, 0));
    if (animator.delayAndCheck(SHORT_DELAY * 20)) return;
    animator.chase(10, CRGB(0, 255, 0), CRGB(200, 200, 200), ANIMATION_TIME, 100);
}

void easter() {
    animator.everyFourth(LED_PINK, LED_LIGHT_BLUE, CRGB(230, 150, 0), CRGB(50, 255, 50));
    animator.show();
    if (animator.delayAndCheck(2000)) return;
    animator.everyFourth(LED_LIGHT_BLUE, CRGB(230, 150, 0), CRGB(50, 255, 50), LED_PINK);
    animator.show();
    animator.delayAndCheck(2000);
}

void movingRainbow() {
    CRGB* rgb = reinterpret_cast<CRGB*>(leds);
    for (int base = 0; base < 256; base += 2) {
        for (int i = 0; i < NUM_LEDS; i++) {
            CHSV hsv((uint8_t)(base + i * 7), 255, 255);
            hsv2rgb_spectrum(hsv, rgb[i]);
        }
        animator.show();
        if (animator.delayAndCheck(20)) return;
    }
}

void solidRainbow() {
    fill_rainbow(reinterpret_cast<CRGB*>(leds), NUM_LEDS, 0, 7);
    animator.show();
    animator.delayAndCheck(1000);
}

void randomCycle() {
    animator.randomFill(randomPalette, 5);
    animator.delayAndCheck(2000);
}

void randomColor() {
    if (lastPattern != pattern) {
        randR = random(0, 255);
        randG = random(0, 255);
        randB = random(0, 255);
    }
    animator.fill(CRGB(randR, randG, randB));
    animator.delayAndCheck(1000);
}

void snowfall() {
    animator.sparkleDecay(CRGB(180, 200, 255), CRGB::Black);
}

void sparkleDecayWhite() {
    animator.sparkleDecay(CRGB(255, 255, 255), CRGB::Black);
}

void sparkleDecayGold() {
    animator.sparkleDecay(CRGB(255, 255, 255), CRGB(60, 30, 0));
}

void pink()       { show_color(LED_PINK); }
void red()        { show_color(CRGB(255, 0, 0)); }
void white()      { show_color(CRGB(255, 255, 255)); }
void blue()       { show_color(CRGB(0, 0, 255)); }
void green()      { show_color(CRGB(0, 255, 0)); }
void yellow()     { show_color(CRGB(255, 180, 0)); }
void orange()     { show_color(LED_ORANGE); }
void lightBlue()  { show_color(LED_LIGHT_BLUE); }
void purple()     { show_color(CRGB(100, 0, 255)); }
void magenta()    { show_color(CRGB(255, 0, 255)); }
void cyan()       { show_color(CRGB(0, 255, 255)); }
void yellowWhite(){ show_color(CRGB(140, 60, 20)); }

// === Pattern Table ===

struct Pattern {
    const char* name;
    void (*run)();
};

const Pattern patterns[] = {
    {"XMas",            xmas},
    {"Fourth",          fourth},
    {"Halloween",       halloweenShow},
    {"Thanksgiving",    thanksgivingShow},
    {"St Pats",         st_pats},
    {"Valentines",      valentines},
    {"Snowfall",        snowfall},
    {"Sparkle White",   sparkleDecayWhite},
    {"Sparkle Gold",    sparkleDecayGold},
    {"Pink",            pink},
    {"Red",             red},
    {"White",           white},
    {"Blue",            blue},
    {"Green",           green},
    {"Yellow",          yellow},
    {"Orange",          orange},
    {"Light Blue",      lightBlue},
    {"Purple",          purple},
    {"Magenta",         magenta},
    {"Cyan",            cyan},
    {"Bears",           bears},
    {"Easter",          easter},
    {"Moving Rainbow",  movingRainbow},
    {"Solid Rainbow",   solidRainbow},
    {"Random Cycle",    randomCycle},
    {"Random Color",    randomColor},
    {"Yellow White",    yellowWhite},
};
const int NUM_PATTERNS = sizeof(patterns) / sizeof(patterns[0]);

bool checkInput() {
    int value = digitalRead(BUTTON_PIN);
    if (buttonState > -1 && value != buttonState) {
        buttonState = value;
        pattern = (pattern + 1) % NUM_PATTERNS;
        return true;
    }
    buttonState = value;
    return false;
}

// === Setup ===
void setup() {
    animator.begin<WS2811, DATA_PIN, RGB>();
    Serial.begin(115200);

    EEPROM.get(0, pattern);
    if (pattern < 0 || pattern >= NUM_PATTERNS) pattern = 0;
    debug("EEPROM Pattern:");
    debug(pattern);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

// === Main Loop ===
void loop() {
    if (lastPattern != pattern) {
        animator.clear();
        Serial.print("Pattern ");
        Serial.print(pattern);
        Serial.print(": ");
        Serial.println(patterns[pattern].name);
        EEPROM.put(0, pattern);
        lastPattern = pattern;
    }

    patterns[pattern].run();
}

// === Debug Functions ===
void debug(const String &s) {
#ifdef DEBUG
    Serial.println(s);
#endif
}

void debug(int i) {
#ifdef DEBUG
    Serial.println(i);
#endif
}
