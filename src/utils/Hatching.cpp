// Hatching.cpp
#include "utils/Hatching.h"
#include <cmath>

void Hatching::parallelFill(AnimationEngine &engine,
                             int16_t x, int16_t y, int16_t w, int16_t h,
                             int density, float angle,
                             uint16_t color) {
    float rad = angle * PI / 180.0f;
    float dx = cos(rad);
    float dy = sin(rad);

    // Perpendicular direction for spacing
    float px = -dy;
    float py = dx;

    // Calculate how many lines we need
    float diagonal = sqrt((float)(w * w + h * h));
    int numLines = (int)(diagonal / density) + 1;

    float centerX = x + w / 2.0f;
    float centerY = y + h / 2.0f;

    for (int i = -numLines / 2; i <= numLines / 2; i++) {
        float offsetX = centerX + px * i * density;
        float offsetY = centerY + py * i * density;

        // Extend line in both directions to ensure it covers the rect
        float x0 = offsetX - dx * diagonal;
        float y0 = offsetY - dy * diagonal;
        float x1 = offsetX + dx * diagonal;
        float y1 = offsetY + dy * diagonal;

        // Clip to rectangle bounds (simple clamp)
        // For proper clipping we'd use Cohen-Sutherland, but clamping works for aesthetics
        int16_t cx0 = constrain((int16_t)x0, x, x + w);
        int16_t cy0 = constrain((int16_t)y0, y, y + h);
        int16_t cx1 = constrain((int16_t)x1, x, x + w);
        int16_t cy1 = constrain((int16_t)y1, y, y + h);

        if (cx0 != cx1 || cy0 != cy1) {
            engine.line(cx0, cy0, cx1, cy1, color);
        }
    }
}

void Hatching::crosshatchFill(AnimationEngine &engine,
                               int16_t x, int16_t y, int16_t w, int16_t h,
                               int density, uint16_t color) {
    parallelFill(engine, x, y, w, h, density, 45.0f, color);
    parallelFill(engine, x, y, w, h, density, -45.0f, color);
}

void Hatching::stippleFill(AnimationEngine &engine,
                            int16_t x, int16_t y, int16_t w, int16_t h,
                            int density, uint16_t color) {
    int area = w * h;
    int numDots = (area * density) / 100;
    numDots = constrain(numDots, 1, 2000);  // Safety cap

    for (int i = 0; i < numDots; i++) {
        int16_t px = x + random(0, w);
        int16_t py = y + random(0, h);
        engine.pixel(px, py, color);
    }
}

void Hatching::circularStipple(AnimationEngine &engine,
                                int16_t cx, int16_t cy, int16_t r,
                                int density, uint16_t color) {
    int area = (int)(PI * r * r);
    int numDots = (area * density) / 100;
    numDots = constrain(numDots, 1, 1500);

    for (int i = 0; i < numDots; i++) {
        // Random point in circle using rejection sampling
        float rx, ry;
        do {
            rx = random(-r, r + 1);
            ry = random(-r, r + 1);
        } while (rx * rx + ry * ry > (float)r * r);

        engine.pixel(cx + (int16_t)rx, cy + (int16_t)ry, color);
    }
}
