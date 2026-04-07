// ModeManager.h — 75/25 Illustrated Poetry / Pure Art cycling
#pragma once

#include <Arduino.h>
#include "modes/ArtMode.h"
#include "modes/BotanistMode.h"
#include "modes/DailyCanvasMode.h"
#include "modes/MoodWindowMode.h"
#include "modes/StoryCanvasMode.h"
#include "modes/AmbientPoetryMode.h"
#include "display/DisplayManager.h"
#include "animation/AnimationEngine.h"
#include "network/TimeManager.h"
#include "network/WeatherClient.h"
#include "network/WiFiManager.h"

class ModeManager {
public:
    ModeManager();

    void init(DisplayManager &dm, TimeManager &tm, WiFiMgr &wifi);
    void update(DisplayManager &dm, TimeManager &tm);
    void nextMode(DisplayManager &dm, TimeManager &tm);

    const char* currentModeName();

private:
    // Art algorithm instances
    BotanistMode _botanist;
    DailyCanvasMode _dailyCanvas;
    MoodWindowMode _moodWindow;
    StoryCanvasMode _storyCanvas;
    AmbientPoetryMode _ambientPoetry;

    AnimationEngine _engine;
    WeatherClient _weatherClient;
    WiFiMgr *_wifi;

    unsigned long _lastTransition;
    unsigned long _lastWeatherFetch;
    bool _firstRun;
    int _cycleCount;
    WeatherData _cachedWeather;

    // Current state
    bool _isIllustratedPoetry;  // true = poetry+art, false = pure art
    int _currentQuoteIndex;
    const char* _currentModeName;

    // Height reserved at the bottom for the weather bar
    static const int WEATHER_BAR_H = 28;
    // Height reserved for the quote text area (with gap before weather bar)
    static const int QUOTE_AREA_H = 90;
    // Y position where quote area starts (10px gap before weather bar)
    static const int QUOTE_AREA_Y = 400 - 28 - 10 - 90;  // = 272

    void runCurrentMode(DisplayManager &dm, TimeManager &tm);
    void runIllustratedPoetry(DisplayManager &dm, TimeManager &tm);
    void runPureArt(DisplayManager &dm, TimeManager &tm);
    void fetchWeather();

    // Map a QuoteTheme to an art algorithm and generate
    ArtMode* getArtForTheme(QuoteTheme theme);
};
