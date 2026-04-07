// StoryCanvasMode.cpp — 📖 Evolving illustrated scenes
#include "modes/StoryCanvasMode.h"
#include "utils/Hatching.h"
#include "config.h"
#include <cmath>

void StoryCanvasMode::init(uint32_t seed) {
    _seed = seed;
    randomSeed(seed);
    _scene = (SceneType)random(0, SCENE_COUNT);
    Serial.printf("[StoryCanvas] Scene: %d, seed: %lu\n", _scene, (unsigned long)seed);
}

void StoryCanvasMode::generate(AnimationEngine &engine) {
    engine.clear();
    switch (_scene) {
        case SCENE_VILLAGE:  generateVillage(engine); break;
        case SCENE_FOREST:   generateForest(engine); break;
        case SCENE_SEASIDE:  generateSeaside(engine); break;
        case SCENE_MOUNTAIN: generateMountain(engine); break;
        case SCENE_GARDEN:   generateGarden(engine); break;
        default:             generateVillage(engine); break;
    }
}

// ── Scene generators ─────────────────────────────────────

void StoryCanvasMode::generateVillage(AnimationEngine &engine) {
    drawSky(engine, true, false, false);
    drawHorizonLine(engine, 200);

    // Ground texture
    Hatching::stippleFill(engine, 0, 200, SCREEN_W, 100, 1);

    // Houses
    int numHouses = 2 + random(0, 3);
    for (int i = 0; i < numHouses; i++) {
        int16_t hx = 40 + i * (SCREEN_W / numHouses);
        drawHouse(engine, hx, 160, 40 + random(0, 20), 35 + random(0, 15));
    }

    // Trees between houses
    for (int i = 0; i < numHouses - 1; i++) {
        int16_t tx = 80 + i * (SCREEN_W / numHouses) + random(0, 30);
        drawTree(engine, tx, 195, 40 + random(0, 20));
    }

    // Birds in sky
    for (int i = 0; i < 3 + random(0, 4); i++)
        drawBird(engine, random(20, SCREEN_W - 20), random(15, 80));

    // Cat on a windowsill (50% chance)
    if (random(0, 2) == 0)
        drawCat(engine, 60 + random(0, 40), 175);

    // Person walking (50% chance)
    if (random(0, 2) == 0)
        drawPerson(engine, random(100, SCREEN_W - 60), 192);

    drawFence(engine, 10, 205, SCREEN_W - 20);
}

void StoryCanvasMode::generateForest(AnimationEngine &engine) {
    drawSky(engine, false, false, true);  // Night forest with moon
    drawHorizonLine(engine, 220);

    // Dense trees
    int numTrees = 5 + random(0, 4);
    for (int i = 0; i < numTrees; i++) {
        int16_t tx = 15 + i * (SCREEN_W / numTrees) + random(-10, 10);
        int16_t th = 50 + random(0, 40);
        drawTree(engine, tx, 215, th);
    }

    // Undergrowth dots
    Hatching::stippleFill(engine, 0, 210, SCREEN_W, 90, 2);

    // Mushrooms
    for (int i = 0; i < random(2, 5); i++) {
        int16_t mx = random(20, SCREEN_W - 20), my = 230 + random(0, 30);
        engine.line(mx, my, mx, my - 8);
        engine.circle(mx, my - 10, 5);
    }

    // Owl in a tree (33% chance)
    if (random(0, 3) == 0) {
        int16_t ox = 60 + random(0, SCREEN_W - 120), oy = 130 + random(0, 30);
        engine.circle(ox, oy, 6);
        engine.fillCircle(ox - 2, oy - 1, 1);
        engine.fillCircle(ox + 2, oy - 1, 1);
        engine.line(ox - 1, oy + 2, ox, oy + 4);
        engine.line(ox + 1, oy + 2, ox, oy + 4);
    }

    // Path
    for (int y = 225; y < SCREEN_H; y += 3) {
        int16_t pw = 15 + (y - 225) / 3;
        int16_t cx = SCREEN_W / 2 + random(-3, 3);
        engine.line(cx - pw, y, cx - pw + 2, y);
        engine.line(cx + pw, y, cx + pw - 2, y);
    }
}

void StoryCanvasMode::generateSeaside(AnimationEngine &engine) {
    drawSky(engine, true, true, false);

    // Sea horizon
    int16_t horizonY = 150;
    drawHorizonLine(engine, horizonY);

    // Waves
    for (int w = 0; w < 4 + random(0, 3); w++) {
        int16_t wy = horizonY + 15 + w * 20 + random(-5, 5);
        drawWave(engine, 0, wy, SCREEN_W);
    }

    // Beach (stippled)
    Hatching::stippleFill(engine, 0, SCREEN_H - 60, SCREEN_W, 60, 2);

    // Boat
    drawBoat(engine, random(80, SCREEN_W - 120), horizonY + 10 + random(0, 20));

    // Birds
    for (int i = 0; i < 2 + random(0, 3); i++)
        drawBird(engine, random(20, SCREEN_W - 20), random(20, horizonY - 20));

    // Lighthouse (33% chance)
    if (random(0, 3) == 0) {
        int16_t lx = SCREEN_W - 50;
        engine.line(lx, SCREEN_H - 60, lx - 8, horizonY + 30);
        engine.line(lx, SCREEN_H - 60, lx + 8, horizonY + 30);
        engine.line(lx - 8, horizonY + 30, lx + 8, horizonY + 30);
        engine.rect(lx - 4, horizonY + 20, 8, 10);
        engine.fillCircle(lx, horizonY + 15, 4);
    }
}

void StoryCanvasMode::generateMountain(AnimationEngine &engine) {
    drawSky(engine, true, true, false);

    // Mountain range (back layer)
    drawMountainShape(engine, SCREEN_W / 4, 200, 120, 160);
    drawMountainShape(engine, SCREEN_W / 2 + 40, 200, 140, 180);
    drawMountainShape(engine, SCREEN_W * 3 / 4, 200, 100, 140);

    // Foreground hills
    drawHorizonLine(engine, 220, true);
    Hatching::stippleFill(engine, 0, 220, SCREEN_W, 80, 1);

    // Pine trees in foreground
    for (int i = 0; i < 3 + random(0, 3); i++) {
        int16_t tx = random(20, SCREEN_W - 20);
        drawTree(engine, tx, 215, 30 + random(0, 20));
    }

    // River/path
    float rx = SCREEN_W / 2.0f;
    for (int y = 220; y < SCREEN_H; y += 2) {
        rx += random(-2, 3);
        int16_t rw = 3 + (y - 220) / 8;
        engine.line((int16_t)rx - rw, y, (int16_t)rx + rw, y);
    }

    drawBird(engine, random(30, SCREEN_W - 30), random(20, 70));
}

void StoryCanvasMode::generateGarden(AnimationEngine &engine) {
    drawSky(engine, false, true, false);
    drawHorizonLine(engine, 180);

    // Garden arch
    int16_t ax = SCREEN_W / 2;
    for (float a = 0; a < PI; a += 0.05f) {
        engine.pixel(ax + (int16_t)(40 * cos(a)), 180 - (int16_t)(50 * sin(a)));
        engine.pixel(ax + (int16_t)(43 * cos(a)), 180 - (int16_t)(50 * sin(a)));
    }
    engine.line(ax - 40, 180, ax - 40, 180 + 20);
    engine.line(ax + 43, 180, ax + 43, 180 + 20);

    // Flowers everywhere
    for (int i = 0; i < 10 + random(0, 8); i++) {
        drawFlower(engine, random(20, SCREEN_W - 20), 185 + random(0, 80));
    }

    // Path through garden
    for (int y = 185; y < SCREEN_H; y += 4) {
        int16_t pw = 12 + (y - 185) / 5;
        int16_t cx = SCREEN_W / 2 + random(-2, 2);
        engine.pixel(cx - pw, y);
        engine.pixel(cx + pw, y);
    }

    // Butterfly (50% chance)
    if (random(0, 2) == 0) {
        int16_t bx = random(60, SCREEN_W - 60), by = random(100, 170);
        engine.line(bx, by, bx - 5, by - 4);
        engine.line(bx, by, bx + 5, by - 4);
        engine.line(bx, by, bx - 4, by + 3);
        engine.line(bx, by, bx + 4, by + 3);
    }

    drawFence(engine, 10, 200, SCREEN_W - 20);
}

// ── Shared drawing helpers ───────────────────────────────

void StoryCanvasMode::drawSky(AnimationEngine &engine, bool clouds, bool sun, bool moon) {
    if (sun) {
        int16_t sx = SCREEN_W - 60 + random(-20, 20);
        int16_t sy = 35 + random(-10, 10);
        engine.circle(sx, sy, 12);
        for (int i = 0; i < 8; i++) {
            float a = i * PI / 4.0f;
            engine.line(sx + (int16_t)(15 * cos(a)), sy + (int16_t)(15 * sin(a)),
                       sx + (int16_t)(22 * cos(a)), sy + (int16_t)(22 * sin(a)));
        }
    }
    if (moon) {
        int16_t mx = 50 + random(0, 30), my = 40 + random(0, 20);
        engine.circle(mx, my, 15);
        engine.fillCircle(mx + 5, my - 3, 13, GxEPD_WHITE);
    }
    if (clouds) {
        for (int c = 0; c < 2 + random(0, 2); c++) {
            int16_t cx = random(30, SCREEN_W - 60);
            int16_t cy = random(20, 60);
            for (int b = 0; b < 3; b++) {
                engine.circle(cx + b * 12, cy, 8 + random(0, 4));
            }
        }
    }
}

void StoryCanvasMode::drawHorizonLine(AnimationEngine &engine, int16_t y, bool wavy) {
    if (wavy) {
        for (int x = 0; x < SCREEN_W - 2; x += 2) {
            int16_t y1 = y + (int16_t)(sin(x * 0.05f) * 3);
            int16_t y2 = y + (int16_t)(sin((x + 2) * 0.05f) * 3);
            engine.line(x, y1, x + 2, y2);
        }
    } else {
        engine.line(0, y, SCREEN_W, y);
    }
}

void StoryCanvasMode::drawHouse(AnimationEngine &engine, int16_t x, int16_t y, int16_t w, int16_t h) {
    // Walls
    engine.rect(x, y, w, h);
    // Roof (triangle)
    engine.line(x - 5, y, x + w / 2, y - h / 2);
    engine.line(x + w + 5, y, x + w / 2, y - h / 2);
    // Door
    engine.rect(x + w / 2 - 4, y + h - 15, 8, 15);
    engine.fillCircle(x + w / 2 + 2, y + h - 8, 1);
    // Window
    engine.rect(x + 5, y + 8, 8, 8);
    engine.line(x + 5, y + 12, x + 13, y + 12);
    engine.line(x + 9, y + 8, x + 9, y + 16);
    // Chimney with smoke
    engine.rect(x + w - 12, y - h / 2 + 5, 6, 15);
    int16_t smokeX = x + w - 9;
    for (int s = 0; s < 3; s++)
        engine.circle(smokeX + random(-3, 3), y - h / 2 - 5 - s * 8, 3 + s);
}

void StoryCanvasMode::drawTree(AnimationEngine &engine, int16_t x, int16_t y, int16_t h) {
    // Trunk
    engine.line(x, y, x, y - h / 2);
    engine.line(x + 2, y, x + 2, y - h / 2);
    // Canopy (overlapping circles)
    engine.circle(x, y - h / 2 - 8, 12);
    engine.circle(x - 8, y - h / 2, 10);
    engine.circle(x + 9, y - h / 2, 10);
    Hatching::circularStipple(engine, x, y - h / 2 - 5, 10, 2);
}

void StoryCanvasMode::drawBird(AnimationEngine &engine, int16_t x, int16_t y) {
    engine.line(x - 4, y, x, y - 3);
    engine.line(x, y - 3, x + 4, y);
}

void StoryCanvasMode::drawCat(AnimationEngine &engine, int16_t x, int16_t y) {
    engine.circle(x, y, 4);                    // Head
    engine.line(x - 4, y - 4, x - 2, y - 1);  // Left ear
    engine.line(x + 4, y - 4, x + 2, y - 1);  // Right ear
    engine.fillCircle(x - 1, y - 1, 1);        // Eyes
    engine.fillCircle(x + 1, y - 1, 1);
    engine.rect(x - 3, y + 4, 6, 8);           // Body
    // Tail curve
    engine.line(x + 3, y + 6, x + 8, y + 3);
    engine.line(x + 8, y + 3, x + 10, y + 5);
}

void StoryCanvasMode::drawPerson(AnimationEngine &engine, int16_t x, int16_t y) {
    engine.circle(x, y - 14, 4);               // Head
    engine.line(x, y - 10, x, y);              // Body
    engine.line(x, y - 7, x - 5, y - 3);      // Left arm
    engine.line(x, y - 7, x + 5, y - 3);      // Right arm
    engine.line(x, y, x - 4, y + 8);           // Left leg
    engine.line(x, y, x + 4, y + 8);           // Right leg
}

void StoryCanvasMode::drawFlower(AnimationEngine &engine, int16_t x, int16_t y) {
    // Stem
    engine.line(x, y, x, y - 10 - random(0, 8));
    // Petals
    int16_t ty = y - 12 - random(0, 8);
    int petals = 4 + random(0, 3);
    for (int p = 0; p < petals; p++) {
        float a = (2.0f * PI * p) / petals;
        engine.line(x, ty, x + (int16_t)(5 * cos(a)), ty + (int16_t)(5 * sin(a)));
    }
    engine.fillCircle(x, ty, 2);
    // Leaf
    engine.line(x, y - 5, x - 4, y - 8);
    engine.line(x - 4, y - 8, x, y - 7);
}

void StoryCanvasMode::drawMountainShape(AnimationEngine &engine, int16_t cx, int16_t baseY,
                                         int16_t height, int16_t width) {
    engine.line(cx - width / 2, baseY, cx, baseY - height);
    engine.line(cx, baseY - height, cx + width / 2, baseY);
    // Snow cap
    int16_t capH = height / 4;
    engine.line(cx - width / 8, baseY - height + capH, cx, baseY - height);
    engine.line(cx, baseY - height, cx + width / 8, baseY - height + capH);
    Hatching::stippleFill(engine, cx - width / 8, baseY - height, width / 4, capH, 3);
}

void StoryCanvasMode::drawWave(AnimationEngine &engine, int16_t startX, int16_t y, int16_t width) {
    for (int x = startX; x < startX + width - 3; x += 3) {
        int16_t y1 = y + (int16_t)(sin(x * 0.08f + random(0, 10) * 0.1f) * 4);
        int16_t y2 = y + (int16_t)(sin((x + 3) * 0.08f + random(0, 10) * 0.1f) * 4);
        engine.line(x, y1, x + 3, y2);
    }
}

void StoryCanvasMode::drawBoat(AnimationEngine &engine, int16_t x, int16_t y) {
    // Hull
    engine.line(x, y + 8, x + 30, y + 8);
    engine.line(x, y + 8, x + 5, y + 14);
    engine.line(x + 30, y + 8, x + 25, y + 14);
    engine.line(x + 5, y + 14, x + 25, y + 14);
    // Mast
    engine.line(x + 15, y + 8, x + 15, y - 15);
    // Sail
    engine.line(x + 15, y - 15, x + 25, y + 5);
    engine.line(x + 15, y + 5, x + 25, y + 5);
}

void StoryCanvasMode::drawFence(AnimationEngine &engine, int16_t x, int16_t y, int16_t width) {
    // Horizontal rails
    engine.line(x, y + 3, x + width, y + 3);
    engine.line(x, y + 10, x + width, y + 10);
    // Vertical posts
    for (int16_t px = x; px <= x + width; px += 12) {
        engine.line(px, y, px, y + 14);
        // Pointed top
        engine.line(px - 1, y + 2, px, y);
        engine.line(px + 1, y + 2, px, y);
    }
}
