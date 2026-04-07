// BotanistMode.h — 🌿 Tiny Botanist: Procedural plant illustrations via L-systems
#pragma once

#include "modes/ArtMode.h"

class BotanistMode : public ArtMode {
public:
    const char* name() override { return "Tiny Botanist"; }
    void init(uint32_t seed) override;
    void generate(AnimationEngine &engine) override;
    int batchSize() override { return 3; }       // Slow, deliberate drawing
    int animationDelayMs() override { return 150; }

private:
    uint32_t _seed;
    int _plantType;  // Which preset to use

    void generateFern(AnimationEngine &engine);
    void generateTree(AnimationEngine &engine);
    void generateWildflower(AnimationEngine &engine);
    void generateSucculent(AnimationEngine &engine);
    void generateVine(AnimationEngine &engine);

    // Draw the specimen label at the bottom
    void drawLabel(AnimationEngine &engine, int specimenNum);

    // Draw a decorative pot/ground line
    void drawPot(AnimationEngine &engine, int16_t cx, int16_t baseY);
};
