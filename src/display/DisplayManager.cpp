// DisplayManager.cpp
#include "DisplayManager.h"

DisplayManager::DisplayManager()
    : display(GxEPD2_420_GDEY042T81(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY))
{
}

void DisplayManager::init() {
    // init(baud, initial, reset_duration, pulldown_rst)
    // Waveshare V2 boards need the shortened reset pulse (param 3 = 2)
    display.init(115200, true, 2, false);
    display.setRotation(1);  // Portrait: 300 wide × 400 tall
    display.setTextWrap(false);
    Serial.println("[Display] Initialized");
}

void DisplayManager::clearScreen() {
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
    Serial.println("[Display] Screen cleared (full refresh)");
}

void DisplayManager::fullRefresh() {
    display.display(false);  // false = full refresh
}

void DisplayManager::partialRefresh(int16_t x, int16_t y, int16_t w, int16_t h) {
    int16_t sx = snapX(x);
    int16_t sw = snapW(x, w);
    // Clamp to screen bounds
    if (sx < 0) sx = 0;
    if (y < 0) y = 0;
    if (sx + sw > SCREEN_W) sw = SCREEN_W - sx;
    if (y + h > SCREEN_H) h = SCREEN_H - y;
    display.displayWindow(sx, y, sw, h);
}

// ── Drawing helpers ──────────────────────────────────────

void DisplayManager::drawPixel(int16_t x, int16_t y, uint16_t color) {
    display.drawPixel(x, y, color);
}

void DisplayManager::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    display.drawLine(x0, y0, x1, y1, color);
}

void DisplayManager::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    display.drawCircle(x, y, r, color);
}

void DisplayManager::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    display.fillCircle(x, y, r, color);
}

void DisplayManager::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.drawRect(x, y, w, h, color);
}

void DisplayManager::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.fillRect(x, y, w, h, color);
}

void DisplayManager::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                   int16_t x2, int16_t y2, uint16_t color) {
    display.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

void DisplayManager::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                   int16_t x2, int16_t y2, uint16_t color) {
    display.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void DisplayManager::setCursor(int16_t x, int16_t y) {
    display.setCursor(x, y);
}

void DisplayManager::setTextSize(uint8_t s) {
    display.setTextSize(s);
}

void DisplayManager::setTextColor(uint16_t c) {
    display.setTextColor(c);
}

void DisplayManager::setFont(const GFXfont *f) {
    display.setFont(f);
}

void DisplayManager::print(const char *text) {
    display.print(text);
}

void DisplayManager::print(int val) {
    display.print(val);
}

// ── Private helpers ──────────────────────────────────────

int16_t DisplayManager::snapX(int16_t x) {
    return (x / 8) * 8;  // Round down to nearest 8
}

int16_t DisplayManager::snapW(int16_t x, int16_t w) {
    int16_t sx = snapX(x);
    int16_t end = x + w;
    int16_t snappedEnd = ((end + 7) / 8) * 8;  // Round up to nearest 8
    return snappedEnd - sx;
}
