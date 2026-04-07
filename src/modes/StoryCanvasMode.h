// StoryCanvasMode.h — 📖 Story Canvas: Evolving illustrated scenes
#pragma once

#include "modes/ArtMode.h"

enum SceneType {
    SCENE_VILLAGE = 0,
    SCENE_FOREST,
    SCENE_SEASIDE,
    SCENE_MOUNTAIN,
    SCENE_GARDEN,
    SCENE_COUNT
};

class StoryCanvasMode : public ArtMode {
public:
    const char* name() override { return "Story Canvas"; }
    void init(uint32_t seed) override;
    void generate(AnimationEngine &engine) override;
    int batchSize() override { return 5; }
    int animationDelayMs() override { return 120; }

private:
    uint32_t _seed;
    SceneType _scene;

    void generateVillage(AnimationEngine &engine);
    void generateForest(AnimationEngine &engine);
    void generateSeaside(AnimationEngine &engine);
    void generateMountain(AnimationEngine &engine);
    void generateGarden(AnimationEngine &engine);

    void drawSky(AnimationEngine &engine, bool clouds, bool sun, bool moon);
    void drawHorizonLine(AnimationEngine &engine, int16_t y, bool wavy = false);
    void drawHouse(AnimationEngine &engine, int16_t x, int16_t y, int16_t w, int16_t h);
    void drawTree(AnimationEngine &engine, int16_t x, int16_t y, int16_t height);
    void drawBird(AnimationEngine &engine, int16_t x, int16_t y);
    void drawCat(AnimationEngine &engine, int16_t x, int16_t y);
    void drawPerson(AnimationEngine &engine, int16_t x, int16_t y);
    void drawFlower(AnimationEngine &engine, int16_t x, int16_t y);
    void drawMountainShape(AnimationEngine &engine, int16_t cx, int16_t baseY, int16_t height, int16_t width);
    void drawWave(AnimationEngine &engine, int16_t startX, int16_t y, int16_t width);
    void drawBoat(AnimationEngine &engine, int16_t x, int16_t y);
    void drawFence(AnimationEngine &engine, int16_t x, int16_t y, int16_t width);
};
