#ifndef LED_ANIMATOR_H
#define LED_ANIMATOR_H

#include <FastLED.h>

// Callback type: returns true to abort current animation
typedef bool (*CheckAbortFn)();

// === Color Types (abstract away FastLED) ===

struct Color {
    uint8_t r, g, b;

    Color() : r(0), g(0), b(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    Color(const CRGB& c) : r(c.r), g(c.g), b(c.b) {}
    operator CRGB() const { return CRGB(r, g, b); }
};

struct ColorHSV {
    uint8_t h, s, v;

    ColorHSV() : h(0), s(0), v(0) {}
    ColorHSV(uint8_t h, uint8_t s, uint8_t v) : h(h), s(s), v(v) {}
    ColorHSV(const CHSV& c) : h(c.h), s(c.s), v(c.v) {}
    operator CHSV() const { return CHSV(h, s, v); }
};

// Convert HSV to RGB
inline Color hsvToRgb(ColorHSV hsv) {
    CRGB rgb;
    hsv2rgb_rainbow(hsv, rgb);
    return rgb;
}

// Blend two colors
inline Color blendColors(Color a, Color b, uint8_t amount) {
    return blend(CRGB(a), CRGB(b), amount);
}

// Common color constants
const Color LED_WHITE(255, 255, 255);
const Color LED_BLACK(0, 0, 0);
const Color LED_PURPLE(30, 0, 60);
const Color LED_ORANGE(80, 20, 0);
const Color LED_LIGHT_BLUE(50, 50, 200);
const Color LED_PINK(255, 30, 30);

class LEDAnimator {
public:
    LEDAnimator(Color* leds, int numLeds, CheckAbortFn checkAbort)
        : _leds(reinterpret_cast<CRGB*>(leds)), _numLeds(numLeds), _checkAbort(checkAbort) {
        // Color has same memory layout as CRGB (3 bytes: r,g,b), so reinterpret_cast is safe
    }

    // Initialize FastLED - call in setup()
    // Usage: animator.begin<WS2811, DATA_PIN, RGB>();
    template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, uint8_t DATA_PIN, EOrder RGB_ORDER = RGB>
    void begin() {
        FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>(_leds, _numLeds);
    }

    // Set global brightness (0-255)
    void setBrightness(uint8_t brightness) {
        FastLED.setBrightness(brightness);
    }

    // Access LED array for direct manipulation
    CRGB& operator[](int i) { return _leds[i]; }
    int numLeds() const { return _numLeds; }

    // === Utilities ===

    void show() {
        FastLED.show();
    }

    void clear() {
        fill(LED_BLACK);
    }

    // Returns true if aborted
    bool delayAndCheck(long ms) {
        while (ms > 0) {
            long chunk = min((long)50, ms);
            delay(chunk);
            ms -= chunk;
            if (_checkAbort()) return true;
        }
        return false;
    }

    // === Pattern Builders (instant, no animation loop) ===

    void fill(CRGB color) {
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = color;
        }
        show();
    }

    void fillHSV(CHSV color) {
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = color;
        }
        show();
    }

    void everyOther(CRGB one, CRGB two) {
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = (i % 2 == 0) ? one : two;
        }
    }

    void everyOtherHSV(CHSV one, CHSV two) {
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = (i % 2 == 0) ? one : two;
        }
    }

    void everyOtherHalf(CRGB color) {
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = (i % 2 == 0) ? color : (CRGB)LED_BLACK;
        }
    }

    void everyThird(CRGB one, CRGB two, CRGB three) {
        CRGB palette[3] = {one, two, three};
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = palette[i % 3];
        }
    }

    void everyFourth(CRGB one, CRGB two, CRGB three, CRGB four) {
        CRGB palette[4] = {one, two, three, four};
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = palette[i % 4];
        }
    }

    void everyNth(int n, CRGB* colors, int numColors) {
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = colors[i % n % numColors];
        }
    }

    void randomFill(Color* palette, int paletteSize) {
        for (int i = 0; i < _numLeds; i++) {
            CRGB color;
            if (i == 0) {
                color = palette[random(0, paletteSize)];
            } else {
                // Ensure adjacent LEDs have different colors
                color = _leds[i - 1];
                while (color == _leds[i - 1]) {
                    color = palette[random(0, paletteSize)];
                }
            }
            _leds[i] = color;
        }
        show();
    }

    // === Animations (loop until aborted or timeout) ===

    // Moving stripes pattern
    // If timeMs is 0, runs until aborted. Returns true if aborted.
    bool chase(int segmentLen, CRGB one, CRGB two, long timeMs = 0, int frameDelayMs = 50) {
        long offset = 0;
        long elapsed = 0;
        while (timeMs == 0 || elapsed < timeMs) {
            offset++;
            for (int i = 0; i < _numLeds; i++) {
                int mod = (i + offset) % (2 * segmentLen);
                _leds[i] = (mod < segmentLen) ? one : two;
            }
            show();
            if (delayAndCheck(frameDelayMs)) return true;
            elapsed += frameDelayMs;
        }
        return false;
    }

    // Three-color chase
    bool chaseThree(int segmentLen, CRGB one, CRGB two, CRGB three, long timeMs = 0, int frameDelayMs = 100) {
        long offset = 0;
        long elapsed = 0;
        while (timeMs == 0 || elapsed < timeMs) {
            offset++;
            for (int i = 0; i < _numLeds; i++) {
                int mod = ((i + offset) / (3 * segmentLen)) % 3;
                _leds[i] = (mod == 0) ? one : (mod == 1) ? two : three;
            }
            show();
            if (delayAndCheck(frameDelayMs)) return true;
            elapsed += frameDelayMs;
        }
        return false;
    }

    // Sparkles that fade toward background color
    bool sparkleDecay(CRGB sparkleColor, CRGB bgColor, int blendRate = 13) {
        fill(bgColor);
        while (true) {
            _leds[random(0, _numLeds)] = sparkleColor;
            for (int i = 0; i < _numLeds; i++) {
                _leds[i] = blend(_leds[i], bgColor, blendRate);
            }
            show();
            if (delayAndCheck(20)) return true;
        }
    }

    // Random colored sparkles fading to black
    bool randomSparkleDecay(int fadeAmount = 10) {
        clear();
        while (true) {
            CHSV color = CHSV(random(255), random(255), 255);
            _leds[random(0, _numLeds)] = color;
            for (int i = 0; i < _numLeds; i++) {
                _leds[i].fadeToBlackBy(fadeAmount);
            }
            show();
            if (delayAndCheck(20)) return true;
        }
    }

    // Breathing/pulsing effect
    bool pulse(CRGB color, long timeMs = 0, int frameDelayMs = 20) {
        int fadeLevel = 0;
        int fadeDirection = 1;
        long elapsed = 0;
        while (timeMs == 0 || elapsed < timeMs) {
            for (int i = 0; i < _numLeds; i++) {
                _leds[i] = color;
                _leds[i].fadeLightBy(fadeLevel);
            }
            show();
            if (delayAndCheck(frameDelayMs)) return true;
            elapsed += frameDelayMs;
            fadeLevel += fadeDirection;
            if (fadeLevel >= 255 || fadeLevel <= 0) {
                fadeDirection = -fadeDirection;
            }
        }
        return false;
    }

    // Single pixel running across the strip
    bool runningLight(CRGB color, int frameDelayMs = 5) {
        clear();
        while (true) {
            for (int i = _numLeds - 1; i > 0; i--) {
                _leds[i] = _leds[i - 1];
            }
            _leds[0] = color;
            show();
            if (delayAndCheck(frameDelayMs)) return true;
        }
    }

    // Alternating pattern that flips periodically
    bool alternate(CRGB one, CRGB two, long timeMs = 0, int switchDelayMs = 1000) {
        long elapsed = 0;
        while (timeMs == 0 || elapsed < timeMs) {
            everyOther(one, two);
            show();
            if (delayAndCheck(switchDelayMs)) return true;
            everyOther(two, one);
            show();
            if (delayAndCheck(switchDelayMs)) return true;
            elapsed += 2 * switchDelayMs;
        }
        return false;
    }

    // Strobe-style flash on full strip
    bool flash(int loops, CRGB color) {
        for (int i = 0; i < loops; i++) {
            fill(color);
            if (delayAndCheck(random(30, 100))) return true;
            fill(CRGB::Black);
            if (delayAndCheck(random(20, 100))) return true;
        }
        return false;
    }

    // Strobe-style flash on every-other LED
    bool flashHalf(int loops, CRGB color) {
        for (int i = 0; i < loops; i++) {
            everyOtherHalf(color);
            show();
            if (delayAndCheck(random(30, 100))) return true;
            everyOtherHalf(CRGB::Black);
            show();
            if (delayAndCheck(random(20, 100))) return true;
        }
        return false;
    }

    // === HSV-aware animations ===

    bool alternateHSV(CHSV one, CHSV two, int frameDelayMs = 100, int frames = 20) {
        for (int f = 0; f < frames; f++) {
            everyOtherHSV(one, two);
            show();
            if (delayAndCheck(frameDelayMs)) return true;
        }
        return false;
    }

    bool chaseHSV(int segmentLen, CHSV one, CHSV two, int frameDelayMs = 50) {
        int offset = 0;
        while (true) {
            offset++;
            for (int i = 0; i < _numLeds; i++) {
                int mod = (i + offset) % (2 * segmentLen);
                _leds[i] = (mod < segmentLen) ? one : two;
            }
            show();
            if (delayAndCheck(frameDelayMs)) return true;
        }
    }

private:
    CRGB* _leds;
    int _numLeds;
    CheckAbortFn _checkAbort;
};

#endif // LED_ANIMATOR_H
