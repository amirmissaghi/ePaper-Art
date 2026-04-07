// BotanistMode.cpp — 🌿 Procedural plant illustrations
#include "modes/BotanistMode.h"
#include "utils/LSystem.h"
#include "utils/Hatching.h"
#include "config.h"

void BotanistMode::init(uint32_t seed) {
    _seed = seed;
    randomSeed(seed);
    _plantType = random(0, 5);
    Serial.printf("[Botanist] Plant type: %d, seed: %lu\n", _plantType, (unsigned long)seed);
}

void BotanistMode::generate(AnimationEngine &engine) {
    engine.clear();

    // Draw border frame
    engine.rect(5, 5, SCREEN_W - 10, SCREEN_H - 10);
    engine.rect(8, 8, SCREEN_W - 16, SCREEN_H - 16);

    switch (_plantType) {
        case 0: generateFern(engine); break;
        case 1: generateTree(engine); break;
        case 2: generateWildflower(engine); break;
        case 3: generateSucculent(engine); break;
        case 4: generateVine(engine); break;
    }

    // Specimen label
    int specimenNum = (_seed % 900) + 100;  // 100–999
    drawLabel(engine, specimenNum);
}

void BotanistMode::generateFern(AnimationEngine &engine) {
    // Barnsley fern-like L-system
    LStochasticRule rules[] = {
        {'F', {"F[+F]F[-F]F", "F[+F]F[-F]", "F[-F]F[+F]F"}, 3},
    };

    String lstr = LSystem::generateStochastic("F", rules, 1, 4);

    float step = 3.0f + random(0, 20) / 10.0f;
    float angle = 20.0f + random(0, 15);

    // Start from bottom center, grow upward (angle = -90 = up)
    LSystem::renderTurtle(lstr, engine,
                          SCREEN_W / 2, SCREEN_H - 70,
                          -90.0f, step, angle);

    drawPot(engine, SCREEN_W / 2, SCREEN_H - 68);
}

void BotanistMode::generateTree(AnimationEngine &engine) {
    // Binary tree with branches
    LRule rules[] = {
        {'A', "B[+A]-A"},
        {'B', "BB"},
    };

    String lstr = LSystem::generate("A", rules, 2, 5);

    float step = 4.0f + random(0, 20) / 10.0f;
    float angle = 30.0f + random(0, 20);

    LSystem::renderTurtle(lstr, engine,
                          SCREEN_W / 2, SCREEN_H - 70,
                          -90.0f, step, angle);

    drawPot(engine, SCREEN_W / 2, SCREEN_H - 68);
}

void BotanistMode::generateWildflower(AnimationEngine &engine) {
    // Flower with petals radiating from stem top
    LStochasticRule rules[] = {
        {'F', {"FF", "F"}, 2},
        {'X', {"F+[[X]-X]-F[-FX]+X", "F-[[X]+X]+F[+FX]-X"}, 2},
    };

    String lstr = LSystem::generateStochastic("X", rules, 2, 4);

    float step = 2.5f + random(0, 15) / 10.0f;
    float angle = 22.0f + random(0, 10);

    LSystem::renderTurtle(lstr, engine,
                          SCREEN_W / 2 - 30, SCREEN_H - 70,
                          -60.0f, step, angle);

    // Add some flower heads (circles at branch tips)
    for (int i = 0; i < 5 + random(0, 5); i++) {
        int16_t fx = SCREEN_W / 2 + random(-80, 80);
        int16_t fy = random(30, 150);
        engine.circle(fx, fy, 4 + random(0, 4));
        // Stippled center
        Hatching::circularStipple(engine, fx, fy, 2, 8);
    }

    drawPot(engine, SCREEN_W / 2, SCREEN_H - 68);
}

void BotanistMode::generateSucculent(AnimationEngine &engine) {
    // Rosette pattern — concentric rings of "leaves"
    int16_t cx = SCREEN_W / 2;
    int16_t cy = SCREEN_H / 2 - 20;

    int numRings = 3 + random(0, 3);

    for (int ring = 0; ring < numRings; ring++) {
        int numLeaves = 5 + ring * 3;
        float radius = 15.0f + ring * 22.0f;
        float leafLen = 18.0f + ring * 8.0f;
        float angleOffset = ring * 15.0f;  // Rotate rings for natural look

        for (int i = 0; i < numLeaves; i++) {
            float a = (360.0f / numLeaves * i + angleOffset) * PI / 180.0f;

            float bx = cx + radius * cos(a);
            float by = cy + radius * sin(a);
            float tx = cx + (radius + leafLen) * cos(a);
            float ty = cy + (radius + leafLen) * sin(a);

            // Leaf shape: two lines forming a pointed oval
            float perpX = -sin(a) * 5;
            float perpY = cos(a) * 5;

            engine.line((int16_t)(bx + perpX), (int16_t)(by + perpY),
                        (int16_t)tx, (int16_t)ty);
            engine.line((int16_t)(bx - perpX), (int16_t)(by - perpY),
                        (int16_t)tx, (int16_t)ty);
            // Base arc
            engine.line((int16_t)(bx + perpX), (int16_t)(by + perpY),
                        (int16_t)(bx - perpX), (int16_t)(by - perpY));
        }
    }

    // Center dot
    engine.fillCircle(cx, cy, 4);

    drawPot(engine, cx, SCREEN_H - 68);
}

void BotanistMode::generateVine(AnimationEngine &engine) {
    // Climbing vine with tendrils
    LStochasticRule rules[] = {
        {'F', {"FF[+F][-F]", "FF[-F][+F]", "FF[+F]"}, 3},
    };

    String lstr = LSystem::generateStochastic("F", rules, 1, 4);

    float step = 3.5f;
    float angle = 25.0f + random(0, 15);

    // Vine grows upward from bottom-left
    LSystem::renderTurtle(lstr, engine,
                          SCREEN_W / 4, SCREEN_H - 70,
                          -80.0f, step, angle);

    // Add small leaves along the vine
    for (int i = 0; i < 8 + random(0, 6); i++) {
        int16_t lx = SCREEN_W / 4 + random(-50, 120);
        int16_t ly = random(40, SCREEN_H - 80);
        // Small leaf: two arced lines
        engine.line(lx, ly, lx + 6, ly - 4);
        engine.line(lx, ly, lx + 6, ly + 4);
        engine.line(lx + 6, ly - 4, lx + 12, ly);
        engine.line(lx + 6, ly + 4, lx + 12, ly);
    }

    drawPot(engine, SCREEN_W / 4, SCREEN_H - 68);
}

void BotanistMode::drawLabel(AnimationEngine &engine, int specimenNum) {
    // Decorative line above label
    int16_t labelY = SCREEN_H - 45;
    engine.line(40, labelY, SCREEN_W - 40, labelY);

    // We can't directly render text via the AnimationEngine (it uses GFX text),
    // so we'll draw decorative elements and the text will be rendered separately
    // in the ModeManager after playback. For now, draw the decorative frame.
    engine.line(40, labelY + 2, SCREEN_W - 40, labelY + 2);

    // Small decorative marks at the corners of the label area
    engine.line(35, labelY - 3, 35, labelY + 35);
    engine.line(SCREEN_W - 35, labelY - 3, SCREEN_W - 35, labelY + 35);
    engine.line(35, labelY + 35, SCREEN_W - 35, labelY + 35);
}

void BotanistMode::drawPot(AnimationEngine &engine, int16_t cx, int16_t baseY) {
    // Simple trapezoid pot
    int16_t potTop = 30;
    int16_t potBottom = 22;
    int16_t potHeight = 18;

    // Pot outline
    engine.line(cx - potTop / 2, baseY, cx + potTop / 2, baseY);                         // Top
    engine.line(cx - potBottom / 2, baseY + potHeight, cx + potBottom / 2, baseY + potHeight);  // Bottom
    engine.line(cx - potTop / 2, baseY, cx - potBottom / 2, baseY + potHeight);           // Left
    engine.line(cx + potTop / 2, baseY, cx + potBottom / 2, baseY + potHeight);           // Right

    // Rim
    engine.line(cx - potTop / 2 - 3, baseY, cx + potTop / 2 + 3, baseY);
    engine.line(cx - potTop / 2 - 3, baseY - 3, cx + potTop / 2 + 3, baseY - 3);
    engine.line(cx - potTop / 2 - 3, baseY, cx - potTop / 2 - 3, baseY - 3);
    engine.line(cx + potTop / 2 + 3, baseY, cx + potTop / 2 + 3, baseY - 3);

    // Soil line (stippled)
    Hatching::stippleFill(engine, cx - potTop / 2 + 2, baseY + 2, potTop - 4, 5, 5);
}
