// AnimationEngine.h — Queues draw commands and replays them with partial refresh
#pragma once

#include <Arduino.h>
#include <vector>
#include "display/DisplayManager.h"

// Callback for overlay content drawn on every animation stage
// (e.g. weather bar, status info)
typedef void (*OverlayFn)(DisplayManager &dm, void *userData);

// Types of draw commands the engine can replay
enum class DrawCmdType : uint8_t {
    PIXEL,
    LINE,
    CIRCLE,
    FILL_CIRCLE,
    RECT,
    FILL_RECT,
    TRIANGLE,
    FILL_TRIANGLE,
};

// A single draw command with all needed parameters
struct DrawCmd {
    DrawCmdType type;
    int16_t x0, y0;       // First point / position
    int16_t x1, y1;       // Second point / size (w,h) / radius in x1
    int16_t x2, y2;       // Third point (triangles only)
    uint16_t color;

    // Bounding box for partial refresh calculation
    void getBounds(int16_t &bx, int16_t &by, int16_t &bw, int16_t &bh) const;
};

class AnimationEngine {
public:
    AnimationEngine();

    // Clear all queued commands
    void clear();

    // Queue draw commands
    void pixel(int16_t x, int16_t y, uint16_t color = GxEPD_BLACK);
    void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color = GxEPD_BLACK);
    void circle(int16_t x, int16_t y, int16_t r, uint16_t color = GxEPD_BLACK);
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color = GxEPD_BLACK);
    void rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color = GxEPD_BLACK);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color = GxEPD_BLACK);
    void triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color = GxEPD_BLACK);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint16_t color = GxEPD_BLACK);

    // Set an overlay function drawn on every animation stage
    // (called during each paged draw, so it appears on every refresh)
    void setOverlay(OverlayFn fn, void *userData = nullptr);

    // Replay all queued commands with animated partial refreshes
    void playback(DisplayManager &dm, int numStages = 100, unsigned long totalDurationMs = 300000);

    // Draw all commands at once (no animation) then full refresh
    void drawAllAtOnce(DisplayManager &dm);

    // Number of queued commands
    size_t count() const { return commands.size(); }

private:
    std::vector<DrawCmd> commands;
    OverlayFn _overlayFn = nullptr;
    void *_overlayData = nullptr;

    // Execute a single draw command on the display
    void executeCmd(DisplayManager &dm, const DrawCmd &cmd);

    // Compute the union bounding box of a range of commands
    void unionBounds(size_t start, size_t end,
                     int16_t &bx, int16_t &by, int16_t &bw, int16_t &bh);
};
