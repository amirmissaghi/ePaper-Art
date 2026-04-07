// ArtMode.h — Abstract base class for all art generation modes
#pragma once

#include <Arduino.h>
#include "animation/AnimationEngine.h"

class ArtMode {
public:
    virtual ~ArtMode() = default;

    // Human-readable name of this mode
    virtual const char* name() = 0;

    // Initialize with a seed (typically date-based for daily uniqueness)
    virtual void init(uint32_t seed) = 0;

    // Generate draw commands into the animation engine
    virtual void generate(AnimationEngine &engine) = 0;

    // How many commands to batch before each partial refresh
    virtual int batchSize() { return ANIM_DEFAULT_BATCH_SIZE; }

    // Delay in ms between refresh batches (controls drawing speed)
    virtual int animationDelayMs() { return ANIM_DEFAULT_DELAY_MS; }
};
