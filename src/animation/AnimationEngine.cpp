// AnimationEngine.cpp
#include "animation/AnimationEngine.h"

// ── DrawCmd bounding box ─────────────────────────────────

void DrawCmd::getBounds(int16_t &bx, int16_t &by, int16_t &bw, int16_t &bh) const {
    switch (type) {
        case DrawCmdType::PIXEL:
            bx = x0; by = y0; bw = 1; bh = 1;
            break;
        case DrawCmdType::LINE:
            bx = min(x0, x1);
            by = min(y0, y1);
            bw = abs(x1 - x0) + 1;
            bh = abs(y1 - y0) + 1;
            break;
        case DrawCmdType::CIRCLE:
        case DrawCmdType::FILL_CIRCLE:
            bx = x0 - x1;  // x1 holds radius
            by = y0 - x1;
            bw = x1 * 2 + 1;
            bh = x1 * 2 + 1;
            break;
        case DrawCmdType::RECT:
        case DrawCmdType::FILL_RECT:
            bx = x0; by = y0; bw = x1; bh = y1;  // x1=w, y1=h
            break;
        case DrawCmdType::TRIANGLE:
        case DrawCmdType::FILL_TRIANGLE:
            bx = min({x0, x1, x2});
            by = min({y0, y1, y2});
            bw = max({x0, x1, x2}) - bx + 1;
            bh = max({y0, y1, y2}) - by + 1;
            break;
    }
}

// ── AnimationEngine ──────────────────────────────────────

AnimationEngine::AnimationEngine() {
    commands.reserve(512);  // Pre-allocate to avoid reallocation
}

void AnimationEngine::clear() {
    commands.clear();
}

void AnimationEngine::pixel(int16_t x, int16_t y, uint16_t color) {
    commands.push_back({DrawCmdType::PIXEL, x, y, 0, 0, 0, 0, color});
}

void AnimationEngine::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    commands.push_back({DrawCmdType::LINE, x0, y0, x1, y1, 0, 0, color});
}

void AnimationEngine::circle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    commands.push_back({DrawCmdType::CIRCLE, x, y, r, 0, 0, 0, color});
}

void AnimationEngine::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    commands.push_back({DrawCmdType::FILL_CIRCLE, x, y, r, 0, 0, 0, color});
}

void AnimationEngine::rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    commands.push_back({DrawCmdType::RECT, x, y, w, h, 0, 0, color});
}

void AnimationEngine::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    commands.push_back({DrawCmdType::FILL_RECT, x, y, w, h, 0, 0, color});
}

void AnimationEngine::triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color) {
    commands.push_back({DrawCmdType::TRIANGLE, x0, y0, x1, y1, x2, y2, color});
}

void AnimationEngine::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                    int16_t x2, int16_t y2, uint16_t color) {
    commands.push_back({DrawCmdType::FILL_TRIANGLE, x0, y0, x1, y1, x2, y2, color});
}

void AnimationEngine::executeCmd(DisplayManager &dm, const DrawCmd &cmd) {
    switch (cmd.type) {
        case DrawCmdType::PIXEL:
            dm.drawPixel(cmd.x0, cmd.y0, cmd.color);
            break;
        case DrawCmdType::LINE:
            dm.drawLine(cmd.x0, cmd.y0, cmd.x1, cmd.y1, cmd.color);
            break;
        case DrawCmdType::CIRCLE:
            dm.drawCircle(cmd.x0, cmd.y0, cmd.x1, cmd.color);
            break;
        case DrawCmdType::FILL_CIRCLE:
            dm.fillCircle(cmd.x0, cmd.y0, cmd.x1, cmd.color);
            break;
        case DrawCmdType::RECT:
            dm.drawRect(cmd.x0, cmd.y0, cmd.x1, cmd.y1, cmd.color);
            break;
        case DrawCmdType::FILL_RECT:
            dm.fillRect(cmd.x0, cmd.y0, cmd.x1, cmd.y1, cmd.color);
            break;
        case DrawCmdType::TRIANGLE:
            dm.drawTriangle(cmd.x0, cmd.y0, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
            break;
        case DrawCmdType::FILL_TRIANGLE:
            dm.fillTriangle(cmd.x0, cmd.y0, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
            break;
    }
}

void AnimationEngine::unionBounds(size_t start, size_t end,
                                   int16_t &bx, int16_t &by, int16_t &bw, int16_t &bh) {
    int16_t minX = SCREEN_W, minY = SCREEN_H, maxX = 0, maxY = 0;
    for (size_t i = start; i < end && i < commands.size(); i++) {
        int16_t cx, cy, cw, ch;
        commands[i].getBounds(cx, cy, cw, ch);
        if (cx < minX) minX = cx;
        if (cy < minY) minY = cy;
        if (cx + cw > maxX) maxX = cx + cw;
        if (cy + ch > maxY) maxY = cy + ch;
    }
    bx = max((int16_t)0, minX);
    by = max((int16_t)0, minY);
    bw = min((int16_t)SCREEN_W, maxX) - bx;
    bh = min((int16_t)SCREEN_H, maxY) - by;
}

void AnimationEngine::setOverlay(OverlayFn fn, void *userData) {
    _overlayFn = fn;
    _overlayData = userData;
}

void AnimationEngine::playback(DisplayManager &dm, int numStages, unsigned long totalDurationMs) {
    if (commands.empty()) return;

    // Clamp stages to command count
    if (numStages > (int)commands.size()) numStages = (int)commands.size();
    if (numStages < 1) numStages = 1;

    int batchSize = (commands.size() + numStages - 1) / numStages;
    unsigned long refreshTimeMs = 380;
    unsigned long totalRefreshTime = refreshTimeMs * numStages;
    unsigned long availableDelay = 0;
    if (totalDurationMs > totalRefreshTime) {
        availableDelay = (totalDurationMs - totalRefreshTime) / numStages;
    }

    Serial.printf("[Anim] Playing %d commands in %d stages (batch=%d, delay=%lums, total~%lus)\n",
                  (int)commands.size(), numStages, batchSize,
                  availableDelay, totalDurationMs / 1000);

    // Write a blank white buffer as the base for partial updates
    dm.display.writeScreenBuffer();

    for (size_t i = 0; i < commands.size(); i += batchSize) {
        size_t end = min(i + (size_t)batchSize, commands.size());

        // Draw all commands up to 'end' + overlay into partial window
        dm.display.setPartialWindow(0, 0, SCREEN_W, SCREEN_H);
        dm.display.firstPage();
        do {
            dm.display.fillScreen(GxEPD_WHITE);
            for (size_t j = 0; j < end; j++) {
                executeCmd(dm, commands[j]);
            }
            // Draw overlay (weather bar) on every stage
            if (_overlayFn) {
                _overlayFn(dm, _overlayData);
            }
        } while (dm.display.nextPage());

        // Pace the animation
        if (availableDelay > 0 && end < commands.size()) {
            delay(availableDelay);
        }
    }

    Serial.println("[Anim] Playback complete");
}

void AnimationEngine::drawAllAtOnce(DisplayManager &dm) {
    dm.display.setFullWindow();
    dm.display.firstPage();
    do {
        dm.display.fillScreen(GxEPD_WHITE);
        for (const auto &cmd : commands) {
            executeCmd(dm, cmd);
        }
    } while (dm.display.nextPage());
    Serial.println("[Anim] Drew all at once (full refresh)");
}
