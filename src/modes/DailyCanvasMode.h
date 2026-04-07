// DailyCanvasMode.h — ✨ Wild generative art algorithms
#pragma once

#include "modes/ArtMode.h"

class DailyCanvasMode : public ArtMode {
public:
    const char* name() override { return "Daily Canvas"; }
    void init(uint32_t seed) override;
    void generate(AnimationEngine &engine) override;

private:
    uint32_t _seed;
    int _algorithm;

    void generateAttractor(AnimationEngine &engine);
    void generateTopography(AnimationEngine &engine);
    void generateMountains(AnimationEngine &engine);
    void generateMoire(AnimationEngine &engine);
    void generateMondrian(AnimationEngine &engine);
    void generateStipple(AnimationEngine &engine);
    void generateMaze(AnimationEngine &engine);
    void generateLissajous(AnimationEngine &engine);
};
