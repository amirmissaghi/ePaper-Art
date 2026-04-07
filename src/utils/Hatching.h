// Hatching.h — Line-based fill patterns (stipple, crosshatch, parallel lines)
#pragma once

#include <Arduino.h>
#include "animation/AnimationEngine.h"

class Hatching {
public:
    // Fill a rectangular region with parallel diagonal lines
    // density: spacing between lines (lower = denser)
    // angle: line angle in degrees (0=horizontal, 45=diagonal, 90=vertical)
    static void parallelFill(AnimationEngine &engine,
                              int16_t x, int16_t y, int16_t w, int16_t h,
                              int density = 4, float angle = 45.0f,
                              uint16_t color = GxEPD_BLACK);

    // Crosshatch: two layers of parallel lines at perpendicular angles
    static void crosshatchFill(AnimationEngine &engine,
                                int16_t x, int16_t y, int16_t w, int16_t h,
                                int density = 5,
                                uint16_t color = GxEPD_BLACK);

    // Stipple: random dots within a region
    // density: approximate number of dots per 100 square pixels
    static void stippleFill(AnimationEngine &engine,
                             int16_t x, int16_t y, int16_t w, int16_t h,
                             int density = 3,
                             uint16_t color = GxEPD_BLACK);

    // Circular stipple: dots arranged in a circle (for shading round objects)
    static void circularStipple(AnimationEngine &engine,
                                 int16_t cx, int16_t cy, int16_t r,
                                 int density = 3,
                                 uint16_t color = GxEPD_BLACK);
};
