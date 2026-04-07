// Noise.h — Perlin noise wrapper using FastLED
#pragma once

#include <Arduino.h>

class Noise {
public:
    // 2D Perlin noise (0-255 range)
    static uint8_t get2D(int x, int y, int scale = 30, int timeOffset = 0);

    // 2D noise mapped to a float angle (0 to 2*PI)
    static float getAngle(int x, int y, int scale = 30, int timeOffset = 0);

    // 2D noise mapped to a custom range
    static float getRange(int x, int y, float minVal, float maxVal,
                           int scale = 30, int timeOffset = 0);

    // 2D Perlin noise with float coordinates, returns -1.0 to 1.0
    static float perlin2D(float x, float y);
};
