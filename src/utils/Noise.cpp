// Noise.cpp
#include "utils/Noise.h"
#define FASTLED_INTERNAL  // Suppress FastLED build banner
#include <FastLED.h>

uint8_t Noise::get2D(int x, int y, int scale, int timeOffset) {
    return inoise8(x * scale, y * scale, timeOffset);
}

float Noise::getAngle(int x, int y, int scale, int timeOffset) {
    uint8_t n = get2D(x, y, scale, timeOffset);
    return (float)n / 255.0f * 2.0f * PI;
}

float Noise::getRange(int x, int y, float minVal, float maxVal, int scale, int timeOffset) {
    uint8_t n = get2D(x, y, scale, timeOffset);
    return minVal + (float)n / 255.0f * (maxVal - minVal);
}

float Noise::perlin2D(float x, float y) {
    // Scale float coords to 16-bit fixed point for inoise16
    // inoise16 returns 0..65535, we map to -1.0..1.0
    uint16_t n = inoise16((uint32_t)(x * 256.0f), (uint32_t)(y * 256.0f));
    return (float)n / 32768.0f - 1.0f;
}
