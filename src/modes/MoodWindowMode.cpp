// MoodWindowMode.cpp — 🌦️ Weather-driven generative art
#include "modes/MoodWindowMode.h"
#include "utils/Noise.h"
#include "utils/Hatching.h"
#include "config.h"
#include <cmath>

void MoodWindowMode::init(uint32_t seed) {
    _seed = seed;
    randomSeed(seed);
    _weather = {0, 0, 0, 0, true, false};  // Invalid until set
}

int MoodWindowMode::tempToDensity() {
    // Map temperature (-20 to 40°C) → density factor (1 to 8)
    float t = constrain(_weather.temperature, -20.0f, 40.0f);
    return (int)map((long)(t * 10), -200, 400, 1, 8);
}

float MoodWindowMode::windToCurve() {
    // Map wind speed (0 to 60 km/h) → curve intensity (0.2 to 2.0)
    float w = constrain(_weather.windSpeed, 0.0f, 60.0f);
    return 0.2f + (w / 60.0f) * 1.8f;
}

void MoodWindowMode::generate(AnimationEngine &engine) {
    engine.clear();

    if (!_weather.valid) {
        Serial.println("[MoodWindow] No weather data, using default");
        generateDefault(engine);
        return;
    }

    Serial.printf("[MoodWindow] Weather code: %d, temp: %.1f, wind: %.1f\n",
                  _weather.weatherCode, _weather.temperature, _weather.windSpeed);

    // Choose visual style based on weather code
    if (WeatherClient::isSnowy(_weather.weatherCode)) {
        generateSnowy(engine);
    } else if (WeatherClient::isRainy(_weather.weatherCode)) {
        generateRainy(engine);
    } else if (WeatherClient::isCloudy(_weather.weatherCode)) {
        generateCloudy(engine);
    } else if (WeatherClient::isClear(_weather.weatherCode)) {
        generateClearSky(engine);
    } else {
        generateDefault(engine);
    }

    // Wind overlay: sweeping curves across the scene
    if (_weather.windSpeed > 10) {
        float curve = windToCurve();
        int numCurves = (int)(_weather.windSpeed / 8);
        for (int i = 0; i < numCurves; i++) {
            float startY = random(20, SCREEN_H - 20);
            float x = 0;
            float y = startY;
            for (int step = 0; step < SCREEN_W; step += 3) {
                float ny = startY + sin((float)step * curve * 0.02f) * 15.0f;
                engine.line((int16_t)x, (int16_t)y, (int16_t)(x + 3), (int16_t)ny);
                x += 3;
                y = ny;
            }
        }
    }
}

void MoodWindowMode::generateClearSky(AnimationEngine &engine) {
    int16_t cx = SCREEN_W / 2;
    int16_t cy = SCREEN_H / 2;

    // Radiating sun pattern
    int numRays = 12 + tempToDensity() * 2;
    float innerR = 25 + random(0, 15);
    float outerR = 80 + random(0, 40);

    // Sun circle
    engine.circle(cx, cy, (int16_t)innerR);
    engine.circle(cx, cy, (int16_t)(innerR - 3));

    // Rays
    for (int i = 0; i < numRays; i++) {
        float angle = (2.0f * PI * i) / numRays;
        float jitter = random(-5, 5) / 100.0f;
        float x0 = cx + (innerR + 5) * cos(angle + jitter);
        float y0 = cy + (innerR + 5) * sin(angle + jitter);
        float rayLen = outerR + random(-10, 20);
        float x1 = cx + rayLen * cos(angle + jitter);
        float y1 = cy + rayLen * sin(angle + jitter);
        engine.line((int16_t)x0, (int16_t)y0, (int16_t)x1, (int16_t)y1);
    }

    // Warmth dots scattered around (more dots = warmer)
    int dots = tempToDensity() * 20;
    for (int i = 0; i < dots; i++) {
        float a = random(0, 360) * PI / 180.0f;
        float r = outerR + 20 + random(0, 60);
        engine.pixel(cx + (int16_t)(r * cos(a)), cy + (int16_t)(r * sin(a)));
    }
}

void MoodWindowMode::generateCloudy(AnimationEngine &engine) {
    // Layered cloud shapes using overlapping rounded forms
    int numClouds = 3 + random(0, 4);

    for (int c = 0; c < numClouds; c++) {
        int16_t baseX = random(30, SCREEN_W - 80);
        int16_t baseY = random(40, SCREEN_H - 80);
        int numBumps = 3 + random(0, 4);

        for (int b = 0; b < numBumps; b++) {
            int16_t bx = baseX + b * (15 + random(0, 10));
            int16_t by = baseY - random(0, 15);
            int16_t r = 15 + random(0, 15);
            engine.circle(bx, by, r);

            // Stipple fill for denser clouds
            if (_weather.humidity > 70) {
                Hatching::circularStipple(engine, bx, by, r - 2, 2);
            }
        }

        // Flat base line
        engine.line(baseX - 10, baseY + 8, baseX + numBumps * 20, baseY + 8);
    }

    // Humidity-driven background dots
    int dots = (int)(_weather.humidity / 10) * 15;
    for (int i = 0; i < dots; i++) {
        engine.pixel(random(0, SCREEN_W), random(0, SCREEN_H));
    }
}

void MoodWindowMode::generateRainy(AnimationEngine &engine) {
    // Rain: vertical lines of varying length
    int rainDensity = tempToDensity() * 8 + 30;

    // Cloud band at top
    for (int x = 0; x < SCREEN_W; x += 3) {
        int16_t cy = 20 + random(0, 15);
        engine.line(x, cy - 5, x, cy + random(0, 8));
    }

    // Rain drops
    for (int i = 0; i < rainDensity; i++) {
        int16_t rx = random(0, SCREEN_W);
        int16_t ry = random(40, SCREEN_H);
        int16_t len = 5 + random(0, 12);
        // Slight angle for wind
        float windOffset = _weather.windSpeed * 0.1f;
        engine.line(rx, ry, rx + (int16_t)windOffset, ry + len);
    }

    // Puddle ripples at bottom
    int numRipples = 3 + random(0, 5);
    for (int i = 0; i < numRipples; i++) {
        int16_t px = random(30, SCREEN_W - 30);
        int16_t py = SCREEN_H - 20 - random(0, 30);
        int numRings = 2 + random(0, 3);
        for (int r = 0; r < numRings; r++) {
            // Horizontal ellipse (ripple)
            int16_t rx = 5 + r * 6;
            int16_t ry = 2 + r * 2;
            for (float a = 0; a < PI; a += 0.15f) {
                int16_t x = px + (int16_t)(rx * cos(a));
                int16_t y = py + (int16_t)(ry * sin(a));
                engine.pixel(x, y);
            }
        }
    }
}

void MoodWindowMode::generateSnowy(AnimationEngine &engine) {
    // Snowflakes: small geometric patterns scattered across the display
    int numFlakes = 20 + random(0, 30);

    for (int i = 0; i < numFlakes; i++) {
        int16_t fx = random(10, SCREEN_W - 10);
        int16_t fy = random(10, SCREEN_H - 10);
        int16_t size = 3 + random(0, 8);

        // 6-pointed snowflake
        for (int arm = 0; arm < 6; arm++) {
            float angle = arm * PI / 3.0f;
            int16_t ex = fx + (int16_t)(size * cos(angle));
            int16_t ey = fy + (int16_t)(size * sin(angle));
            engine.line(fx, fy, ex, ey);

            // Small branches on larger flakes
            if (size > 5) {
                float branchAngle1 = angle + PI / 6.0f;
                float branchAngle2 = angle - PI / 6.0f;
                int16_t mx = fx + (int16_t)(size * 0.6f * cos(angle));
                int16_t my = fy + (int16_t)(size * 0.6f * sin(angle));
                int16_t bl = size / 3;
                engine.line(mx, my, mx + (int16_t)(bl * cos(branchAngle1)),
                           my + (int16_t)(bl * sin(branchAngle1)));
                engine.line(mx, my, mx + (int16_t)(bl * cos(branchAngle2)),
                           my + (int16_t)(bl * sin(branchAngle2)));
            }
        }
    }

    // Ground snow: stippled bottom area
    Hatching::stippleFill(engine, 0, SCREEN_H - 40, SCREEN_W, 40, 4);
    // Snow line
    for (int x = 0; x < SCREEN_W; x += 2) {
        int16_t y = SCREEN_H - 40 + random(-5, 5);
        engine.pixel(x, y);
    }
}

void MoodWindowMode::generateDefault(AnimationEngine &engine) {
    // Abstract Perlin noise pattern when no weather data available
    int timeOff = (int)(_seed % 10000);
    int density = 4;

    for (int y = 0; y < SCREEN_H; y += 4) {
        for (int x = 0; x < SCREEN_W; x += 4) {
            uint8_t n = Noise::get2D(x, y, 25, timeOff);
            if (n > 180) {
                engine.pixel(x, y);
            }
            if (n > 220) {
                engine.pixel(x + 1, y);
                engine.pixel(x, y + 1);
            }
        }
    }
}
