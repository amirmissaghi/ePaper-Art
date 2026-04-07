// DailyCanvasMode.cpp — ✨ Wild generative art
#include "modes/DailyCanvasMode.h"
#include "utils/Noise.h"
#include "config.h"
#include <cmath>

void DailyCanvasMode::init(uint32_t seed) {
    _seed = seed;
    randomSeed(seed);
    _algorithm = random(0, 8);
    Serial.printf("[DailyCanvas] Algorithm: %d, seed: %lu\n", _algorithm, (unsigned long)seed);
}

void DailyCanvasMode::generate(AnimationEngine &engine) {
    engine.clear();

    switch (_algorithm) {
        case 0: generateAttractor(engine); break;
        case 1: generateTopography(engine); break;
        case 2: generateMountains(engine); break;
        case 3: generateMoire(engine); break;
        case 4: generateMondrian(engine); break;
        case 5: generateStipple(engine); break;
        case 6: generateMaze(engine); break;
        case 7: generateLissajous(engine); break;
    }
}

// ══════════════════════════════════════════════════════════
//  0. CLIFFORD ATTRACTOR — Swirling smoke/galaxy
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateAttractor(AnimationEngine &engine) {
    // Clifford attractor: x' = sin(a*y) + c*cos(a*x)
    //                     y' = sin(b*x) + d*cos(b*y)
    float a = -2.0f + random(0, 400) / 100.0f;  // -2.0 to 2.0
    float b = -2.0f + random(0, 400) / 100.0f;
    float c = -2.0f + random(0, 400) / 100.0f;
    float d = -2.0f + random(0, 400) / 100.0f;

    Serial.printf("[DailyCanvas] Attractor: a=%.2f b=%.2f c=%.2f d=%.2f\n", a, b, c, d);

    float x = 0.1f, y = 0.1f;
    int iterations = 2500;

    for (int i = 0; i < iterations; i++) {
        float nx = sin(a * y) + c * cos(a * x);
        float ny = sin(b * x) + d * cos(b * y);
        x = nx;
        y = ny;

        // Map attractor space (-3..3) to screen
        int16_t sx = (int16_t)((x + 3.0f) / 6.0f * (SCREEN_W - 20) + 10);
        int16_t sy = (int16_t)((y + 3.0f) / 6.0f * (SCREEN_H - 60) + 10);

        if (sx >= 0 && sx < SCREEN_W && sy >= 0 && sy < SCREEN_H - 30) {
            engine.pixel(sx, sy);
        }
    }
}

// ══════════════════════════════════════════════════════════
//  1. TOPOGRAPHIC CONTOURS — Perlin noise as contour map
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateTopography(AnimationEngine &engine) {
    float scale = 0.008f + random(0, 8) / 1000.0f;
    int offsetX = random(0, 10000);
    int offsetY = random(0, 10000);
    int numContours = 8 + random(0, 6);
    int artH = SCREEN_H - 30;  // leave room for weather bar

    Serial.printf("[DailyCanvas] Topo: scale=%.4f contours=%d\n", scale, numContours);

    // For each contour level, trace the boundary
    for (int level = 0; level < numContours; level++) {
        float threshold = (float)(level + 1) / (numContours + 1);

        // Scan grid, draw pixels where noise crosses threshold
        for (int y = 5; y < artH - 5; y += 2) {
            for (int x = 5; x < SCREEN_W - 5; x += 2) {
                float n1 = Noise::perlin2D((x + offsetX) * scale, (y + offsetY) * scale);
                float n2 = Noise::perlin2D((x + 2 + offsetX) * scale, (y + offsetY) * scale);
                float n3 = Noise::perlin2D((x + offsetX) * scale, (y + 2 + offsetY) * scale);

                // Normalize from [-1,1] to [0,1]
                n1 = (n1 + 1.0f) * 0.5f;
                n2 = (n2 + 1.0f) * 0.5f;
                n3 = (n3 + 1.0f) * 0.5f;

                // Edge detection: place pixel where contour crosses
                bool cross_h = (n1 < threshold) != (n2 < threshold);
                bool cross_v = (n1 < threshold) != (n3 < threshold);

                if (cross_h || cross_v) {
                    engine.pixel(x, y);
                }
            }
        }
    }
}

// ══════════════════════════════════════════════════════════
//  2. JOY DIVISION MOUNTAINS — Stacked ridgelines
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateMountains(AnimationEngine &engine) {
    int numRidges = 20 + random(0, 15);
    float noiseScale = 0.01f + random(0, 10) / 1000.0f;
    int offsetX = random(0, 10000);
    int artH = SCREEN_H - 30;
    int margin = 15;

    float ridgeSpacing = (float)(artH - margin * 2) / numRidges;

    Serial.printf("[DailyCanvas] Mountains: %d ridges\n", numRidges);

    for (int ridge = 0; ridge < numRidges; ridge++) {
        float baseY = margin + ridge * ridgeSpacing;

        int16_t prevX = margin;
        // Amplitude varies: bigger in the middle, smaller at edges
        float midFactor = 1.0f - fabsf((float)ridge / numRidges - 0.5f) * 2.0f;
        float amplitude = 10.0f + midFactor * 35.0f;

        float noise_y = ridge * 0.8f;
        float prevPy = baseY;

        for (int x = margin; x < SCREEN_W - margin; x += 2) {
            float n = Noise::perlin2D((x + offsetX) * noiseScale, noise_y);
            // Add second octave for detail
            float n2 = Noise::perlin2D((x + offsetX) * noiseScale * 2.5f, noise_y * 2.5f) * 0.3f;

            float py = baseY - (n + n2) * amplitude;
            // Clamp so ridges don't go below their baseline
            if (py > baseY) py = baseY;

            engine.line(prevX, (int16_t)prevPy, x, (int16_t)py);
            prevX = x;
            prevPy = py;
        }
    }
}

// ══════════════════════════════════════════════════════════
//  3. MOIRÉ INTERFERENCE — Overlapping circle patterns
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateMoire(AnimationEngine &engine) {
    // Two or three sets of concentric circles/lines offset from each other
    int numCenters = 2 + random(0, 2);
    int artH = SCREEN_H - 30;

    struct Center { float x, y; };
    Center centers[4];

    for (int i = 0; i < numCenters; i++) {
        centers[i].x = random(50, SCREEN_W - 50);
        centers[i].y = random(50, artH - 50);
    }

    int spacing = 6 + random(0, 6);
    int maxRadius = 200;

    Serial.printf("[DailyCanvas] Moire: %d centers, spacing=%d\n", numCenters, spacing);

    for (int c = 0; c < numCenters; c++) {
        for (int r = spacing; r < maxRadius; r += spacing) {
            // Draw circle as line segments for animation
            int segments = max(16, r / 2);
            float step = 2.0f * PI / segments;
            float prevA = 0;
            int16_t px = (int16_t)(centers[c].x + r);
            int16_t py = (int16_t)(centers[c].y);

            for (int s = 1; s <= segments; s++) {
                float a = s * step;
                int16_t nx = (int16_t)(centers[c].x + r * cos(a));
                int16_t ny = (int16_t)(centers[c].y + r * sin(a));

                // Only draw if at least partially on screen
                if ((px >= 0 || nx >= 0) && (px < SCREEN_W || nx < SCREEN_W) &&
                    (py >= 0 || ny >= 0) && (py < artH || ny < artH)) {
                    engine.line(px, py, nx, ny);
                }
                px = nx;
                py = ny;
            }
        }
    }
}

// ══════════════════════════════════════════════════════════
//  4. RECURSIVE MONDRIAN — Random rectangular subdivision
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateMondrian(AnimationEngine &engine) {
    struct Rect { int x, y, w, h; };
    Rect stack[128];
    int top = 0;
    int artH = SCREEN_H - 30;

    // Start with full canvas
    stack[top++] = {10, 10, SCREEN_W - 20, artH - 20};

    int minSize = 25 + random(0, 20);

    Serial.printf("[DailyCanvas] Mondrian: minSize=%d\n", minSize);

    while (top > 0) {
        Rect r = stack[--top];

        // Draw this rectangle
        engine.rect(r.x, r.y, r.w, r.h);

        // Decide whether to subdivide
        bool canSplitH = r.w > minSize * 2;
        bool canSplitV = r.h > minSize * 2;

        if (!canSplitH && !canSplitV) {
            // Terminal rectangle — maybe fill it
            if (random(0, 100) < 25) {
                // Fill with black (quarter of terminal rects)
                engine.fillRect(r.x + 2, r.y + 2, r.w - 4, r.h - 4);
            } else if (random(0, 100) < 20) {
                // Cross-hatch pattern
                for (int i = r.x + 4; i < r.x + r.w - 4; i += 4) {
                    engine.line(i, r.y + 3, i, r.y + r.h - 3);
                }
            }
            continue;
        }

        // Choose split direction
        bool splitHorizontal;
        if (canSplitH && canSplitV) {
            splitHorizontal = (r.w > r.h) ? (random(0, 100) < 70) : (random(0, 100) < 30);
        } else {
            splitHorizontal = canSplitH;
        }

        if (splitHorizontal) {
            int split = r.x + minSize + random(0, r.w - minSize * 2);
            if (top < 126) {
                stack[top++] = {r.x, r.y, split - r.x, r.h};
                stack[top++] = {split, r.y, r.x + r.w - split, r.h};
            }
        } else {
            int split = r.y + minSize + random(0, r.h - minSize * 2);
            if (top < 126) {
                stack[top++] = {r.x, r.y, r.w, split - r.y};
                stack[top++] = {r.x, split, r.w, r.y + r.h - split};
            }
        }
    }
}

// ══════════════════════════════════════════════════════════
//  5. STIPPLE FIELD — Noise-density stippling
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateStipple(AnimationEngine &engine) {
    float scale = 0.015f + random(0, 10) / 1000.0f;
    int offsetX = random(0, 10000);
    int offsetY = random(0, 10000);
    int artH = SCREEN_H - 30;

    // Add a secondary noise layer for fine detail
    float scale2 = scale * 3.0f;
    int off2X = random(0, 10000);
    int off2Y = random(0, 10000);

    Serial.printf("[DailyCanvas] Stipple: scale=%.4f\n", scale);

    for (int y = 5; y < artH - 5; y += 3) {
        for (int x = 5; x < SCREEN_W - 5; x += 3) {
            float n = Noise::perlin2D((x + offsetX) * scale, (y + offsetY) * scale);
            float n2 = Noise::perlin2D((x + off2X) * scale2, (y + off2Y) * scale2);

            // Combine: primary shape + fine texture
            float combined = (n * 0.7f + n2 * 0.3f + 1.0f) * 0.5f;  // 0..1

            // Higher noise = denser stippling
            // Use threshold with jitter for organic feel
            float threshold = (float)random(0, 100) / 100.0f;
            if (combined > threshold * 0.8f + 0.1f) {
                // Vary dot size based on density
                if (combined > 0.75f) {
                    engine.fillCircle(x, y, 1);
                } else {
                    engine.pixel(x, y);
                }
            }
        }
    }
}

// ══════════════════════════════════════════════════════════
//  6. MAZE GENERATION — Recursive backtracker (stack-safe)
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateMaze(AnimationEngine &engine) {
    int artH = SCREEN_H - 30;
    int cellSize = 14 + random(0, 8);  // 14-21 px cells (bigger = fewer cells = less memory)
    int cols = (SCREEN_W - 20) / cellSize;
    int rows = (artH - 20) / cellSize;
    int startX = (SCREEN_W - cols * cellSize) / 2;
    int startY = (artH - rows * cellSize) / 2;

    // Hard cap to keep memory safe
    if (cols > 18) cols = 18;
    if (rows > 22) rows = 22;

    int totalCells = cols * rows;

    Serial.printf("[DailyCanvas] Maze: %dx%d cells, size=%d\n", cols, rows, cellSize);

    // Pack walls + visited into a single byte per cell (heap allocated)
    // Bits 0-3: walls (top, right, bottom, left), Bit 4: visited
    uint8_t *cells = (uint8_t *)malloc(totalCells);
    if (!cells) {
        Serial.println("[DailyCanvas] Maze: malloc failed, skipping");
        return;
    }
    memset(cells, 0x0F, totalCells);  // All walls, not visited

    #define MAZE_IDX(r,c) ((r) * cols + (c))
    #define MAZE_VISITED 0x10

    // Backtracker stack (heap allocated, max = totalCells)
    int16_t *stackR = (int16_t *)malloc(totalCells * sizeof(int16_t));
    int16_t *stackC = (int16_t *)malloc(totalCells * sizeof(int16_t));
    if (!stackR || !stackC) {
        free(cells);
        if (stackR) free(stackR);
        if (stackC) free(stackC);
        Serial.println("[DailyCanvas] Maze: stack malloc failed");
        return;
    }

    int top = 0;
    int cr = 0, cc = 0;
    cells[MAZE_IDX(cr, cc)] |= MAZE_VISITED;
    stackR[top] = cr;
    stackC[top] = cc;
    top++;

    // Direction offsets: top, right, bottom, left
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};
    const uint8_t wallBit[] = {1, 2, 4, 8};
    const uint8_t oppositeWall[] = {4, 8, 1, 2};

    while (top > 0) {
        int neighbors[4];
        int numNeighbors = 0;

        for (int d = 0; d < 4; d++) {
            int nr = cr + dr[d];
            int nc = cc + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !(cells[MAZE_IDX(nr, nc)] & MAZE_VISITED)) {
                neighbors[numNeighbors++] = d;
            }
        }

        if (numNeighbors > 0) {
            int d = neighbors[random(0, numNeighbors)];
            int nr = cr + dr[d];
            int nc = cc + dc[d];

            cells[MAZE_IDX(cr, cc)] &= ~wallBit[d];
            cells[MAZE_IDX(nr, nc)] &= ~oppositeWall[d];
            cells[MAZE_IDX(nr, nc)] |= MAZE_VISITED;

            stackR[top] = nr;
            stackC[top] = nc;
            top++;
            cr = nr;
            cc = nc;
        } else {
            top--;
            if (top > 0) {
                cr = stackR[top - 1];
                cc = stackC[top - 1];
            }
        }
    }

    // Draw the maze walls
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int x = startX + c * cellSize;
            int y = startY + r * cellSize;
            uint8_t w = cells[MAZE_IDX(r, c)] & 0x0F;

            if (w & 1) engine.line(x, y, x + cellSize, y);
            if (w & 2) engine.line(x + cellSize, y, x + cellSize, y + cellSize);
            if (w & 4) engine.line(x, y + cellSize, x + cellSize, y + cellSize);
            if (w & 8) engine.line(x, y, x, y + cellSize);
        }
    }

    // Entry/exit markers
    engine.fillCircle(startX + cellSize / 2, startY + cellSize / 2, 2);
    engine.fillCircle(startX + (cols - 1) * cellSize + cellSize / 2,
                      startY + (rows - 1) * cellSize + cellSize / 2, 2);

    free(cells);
    free(stackR);
    free(stackC);

    #undef MAZE_IDX
    #undef MAZE_VISITED
}

// ══════════════════════════════════════════════════════════
//  7. LISSAJOUS CURVES — Harmonic oscilloscope art
// ══════════════════════════════════════════════════════════
void DailyCanvasMode::generateLissajous(AnimationEngine &engine) {
    int16_t cx = SCREEN_W / 2;
    int16_t cy = (SCREEN_H - 30) / 2;
    int artH = SCREEN_H - 30;

    // Random harmonic ratios — pick integers for closed curves
    int freqA = 1 + random(0, 7);   // 1-7
    int freqB = 1 + random(0, 7);   // 1-7
    float phase = random(0, 314) / 100.0f;  // 0 to PI

    float ampX = (SCREEN_W - 30) / 2.0f;
    float ampY = (artH - 30) / 2.0f;

    // Number of curves to overlay
    int numCurves = 1 + random(0, 4);

    Serial.printf("[DailyCanvas] Lissajous: %d curves, base freq=%d/%d\n",
                  numCurves, freqA, freqB);

    for (int curve = 0; curve < numCurves; curve++) {
        int fA = freqA + curve;
        int fB = freqB + curve * 2;
        float ph = phase + curve * 0.5f;
        float aX = ampX - curve * 15;
        float aY = ampY - curve * 10;
        if (aX < 20) aX = 20;
        if (aY < 20) aY = 20;

        int steps = 600 + random(0, 400);
        float dt = 2.0f * PI * max(fA, fB) / steps;

        float prevX = cx + aX * sin(0);
        float prevY = cy + aY * sin(ph);

        for (int i = 1; i <= steps; i++) {
            float t = i * dt;
            float x = cx + aX * sin(fA * t);
            float y = cy + aY * sin(fB * t + ph);

            engine.line((int16_t)prevX, (int16_t)prevY, (int16_t)x, (int16_t)y);
            prevX = x;
            prevY = y;
        }
    }

    // Optional: add dots at key intersections
    for (int i = 0; i < 8; i++) {
        float t = i * PI / 4;
        int16_t x = cx + (int16_t)(ampX * 0.3f * sin(freqA * t));
        int16_t y = cy + (int16_t)(ampY * 0.3f * sin(freqB * t + phase));
        engine.fillCircle(x, y, 2);
    }
}
