// DisplayManager.h — Thin wrapper around GxEPD2 for the Waveshare 4.2" V2
#pragma once

#include <GxEPD2_BW.h>
#include "config.h"

// Driver for Waveshare 4.2" B/W V2 (GDEY042T81)
// If this doesn't work, alternatives to try:
//   GxEPD2_420_GDEW042T2
//   GxEPD2_420
typedef GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> EPD_Display;

class DisplayManager {
public:
    EPD_Display display;

    DisplayManager();

    void init();

    // Full-screen operations
    void clearScreen();
    void fullRefresh();

    // Partial refresh of a rectangular region (x and w snapped to 8-pixel boundaries)
    void partialRefresh(int16_t x, int16_t y, int16_t w, int16_t h);

    // Drawing helpers (delegated to Adafruit GFX underneath)
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint16_t color);
    void setCursor(int16_t x, int16_t y);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setFont(const GFXfont *f);
    void print(const char *text);
    void print(int val);

    // Get the raw GFX canvas for direct drawing (used by animation engine)
    EPD_Display& raw() { return display; }

private:
    // Snap x-coordinate down and width up to 8-pixel boundary
    int16_t snapX(int16_t x);
    int16_t snapW(int16_t x, int16_t w);
};
