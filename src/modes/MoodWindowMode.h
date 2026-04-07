// MoodWindowMode.h — 🌦️ Mood Window: Weather-driven generative art
#pragma once

#include "modes/ArtMode.h"
#include "network/WeatherClient.h"

class MoodWindowMode : public ArtMode {
public:
    const char* name() override { return "Mood Window"; }
    void init(uint32_t seed) override;
    void generate(AnimationEngine &engine) override;
    int batchSize() override { return 10; }
    int animationDelayMs() override { return 50; }

    // Must be called before generate() to provide weather data
    void setWeatherData(const WeatherData &data) { _weather = data; }

private:
    uint32_t _seed;
    WeatherData _weather;

    void generateClearSky(AnimationEngine &engine);
    void generateCloudy(AnimationEngine &engine);
    void generateRainy(AnimationEngine &engine);
    void generateSnowy(AnimationEngine &engine);
    void generateWindy(AnimationEngine &engine);
    void generateDefault(AnimationEngine &engine);  // Fallback if no weather data

    // Temperature affects the overall density/complexity
    int tempToDensity();
    // Wind affects curve intensity
    float windToCurve();
};
