// ModeManager.cpp — 75/25 Illustrated Poetry / Pure Art
#include "modes/ModeManager.h"
#include "config.h"
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// ── Overlay contexts ─────────────────────────────────────

struct OverlayCtx {
    WeatherData *weather;
    TimeManager *timeMgr;
    AmbientPoetryMode *poetry;
    bool showQuote;
    int quoteY;
    int quoteH;
};

// Static callback — drawn on every animation stage
static void overlayCallback(DisplayManager &dm, void *userData) {
    OverlayCtx *ctx = (OverlayCtx *)userData;

    // ── Weather bar (always) ──
    int16_t barY = SCREEN_H - 28;
    dm.display.fillRect(0, barY, SCREEN_W, 28, GxEPD_WHITE);
    dm.display.drawLine(10, barY + 2, SCREEN_W - 10, barY + 2, GxEPD_BLACK);

    dm.display.setFont(&FreeSans9pt7b);
    dm.display.setTextColor(GxEPD_BLACK);

    ctx->timeMgr->update();
    String timeStr = ctx->timeMgr->getTimeString();
    dm.display.setCursor(12, barY + 20);
    dm.display.print(timeStr.c_str());

    if (ctx->weather->valid) {
        char tempStr[16];
        float tempF = ctx->weather->temperature * 9.0f / 5.0f + 32.0f;
        snprintf(tempStr, sizeof(tempStr), "%.0f%cF", tempF, 0xB0);
        const char *cond = WeatherClient::weatherCodeToString(ctx->weather->weatherCode);
        char weatherStr[32];
        snprintf(weatherStr, sizeof(weatherStr), "%s  %s", tempStr, cond);
        int16_t x1, y1;
        uint16_t tw, th;
        dm.display.getTextBounds(weatherStr, 0, 0, &x1, &y1, &tw, &th);
        dm.display.setCursor(SCREEN_W - tw - 12, barY + 20);
        dm.display.print(weatherStr);
    } else {
        int16_t x1, y1;
        uint16_t tw, th;
        dm.display.getTextBounds("offline", 0, 0, &x1, &y1, &tw, &th);
        dm.display.setCursor(SCREEN_W - tw - 12, barY + 20);
        dm.display.print("offline");
    }

    // ── Quote area (only for illustrated poetry) ──
    if (ctx->showQuote && ctx->poetry) {
        // White background for quote area
        dm.display.fillRect(0, ctx->quoteY, SCREEN_W, ctx->quoteH, GxEPD_WHITE);
        ctx->poetry->renderText(dm, ctx->quoteY, ctx->quoteH);
    }
}

// ── ModeManager ──────────────────────────────────────────

ModeManager::ModeManager()
    : _lastTransition(0), _lastWeatherFetch(0),
      _firstRun(true), _cycleCount(0), _wifi(nullptr),
      _isIllustratedPoetry(true), _currentQuoteIndex(0),
      _currentModeName("Initializing") {
    _cachedWeather = {0, 0, 0, 0, true, false};
}

void ModeManager::init(DisplayManager &dm, TimeManager &tm, WiFiMgr &wifi) {
    Serial.println("[ModeManager] Initializing...");
    _wifi = &wifi;
    _lastTransition = millis();
    _firstRun = true;

    fetchWeather();
    runCurrentMode(dm, tm);
}

void ModeManager::update(DisplayManager &dm, TimeManager &tm) {
    unsigned long elapsed = millis() - _lastTransition;
    if (elapsed >= MODE_CYCLE_INTERVAL_MS) {
        nextMode(dm, tm);
    }
}

void ModeManager::nextMode(DisplayManager &dm, TimeManager &tm) {
    _lastTransition = millis();
    _cycleCount++;

    // Re-fetch weather periodically
    unsigned long weatherAge = millis() - _lastWeatherFetch;
    if (weatherAge > 1200000 || !_cachedWeather.valid) {
        fetchWeather();
    }

    dm.clearScreen();
    delay(500);

    runCurrentMode(dm, tm);
}

const char* ModeManager::currentModeName() {
    return _currentModeName;
}

void ModeManager::fetchWeather() {
    if (!_wifi) return;

    // Wake WiFi briefly for data fetch
    bool connected = _wifi->wake();
    if (connected) {
        Serial.println("[ModeManager] Fetching weather...");
        _cachedWeather = _weatherClient.fetch();
        _lastWeatherFetch = millis();
    } else {
        Serial.println("[ModeManager] WiFi failed, using cached weather");
    }

    // Turn WiFi back off immediately
    _wifi->sleep();
}

void ModeManager::runCurrentMode(DisplayManager &dm, TimeManager &tm) {
    tm.update();

    // 75/25 weighted random: illustrated poetry vs pure art
    uint32_t roll = random(0, 100);
    _isIllustratedPoetry = (roll < 75);

    Serial.println("════════════════════════════════════");
    if (_isIllustratedPoetry) {
        Serial.printf("[ModeManager] Cycle %d: ILLUSTRATED POETRY (roll=%lu)\n",
                      _cycleCount, (unsigned long)roll);
        runIllustratedPoetry(dm, tm);
    } else {
        Serial.printf("[ModeManager] Cycle %d: PURE ART (roll=%lu)\n",
                      _cycleCount, (unsigned long)roll);
        runPureArt(dm, tm);
    }
    Serial.println("════════════════════════════════════");
}

ArtMode* ModeManager::getArtForTheme(QuoteTheme theme) {
    switch (theme) {
        case QuoteTheme::NATURE:   return &_botanist;
        case QuoteTheme::SKY:      return &_moodWindow;
        case QuoteTheme::WATER:    // Flow field / circle packing from DailyCanvas
        case QuoteTheme::ABSTRACT: return &_dailyCanvas;
        case QuoteTheme::SCENE:    return &_storyCanvas;
        default:                   return &_dailyCanvas;
    }
}

void ModeManager::runIllustratedPoetry(DisplayManager &dm, TimeManager &tm) {
    // Use hardware RNG + time entropy for truly random quote selection
    uint32_t entropy = esp_random();  // hardware RNG
    randomSeed(entropy ^ millis() ^ (_cycleCount * 2654435761UL));
    _currentQuoteIndex = random(0, AmbientPoetryMode::getQuoteCount());
    const Quote &quote = AmbientPoetryMode::getQuote(_currentQuoteIndex);

    // Seed for art: combine quote hash with entropy for unique pairing
    uint32_t seed = AmbientPoetryMode::hashQuote(quote.text) ^ entropy;

    // Get the art algorithm matched to the quote's theme
    ArtMode *artMode = getArtForTheme(quote.theme);
    _currentModeName = artMode->name();

    Serial.printf("[ModeManager] Quote %d → theme %d → art '%s' → seed %lu\n",
                  _currentQuoteIndex, (int)quote.theme,
                  artMode->name(), (unsigned long)seed);
    Serial.printf("[ModeManager] Free heap: %d bytes\n", ESP.getFreeHeap());

    // Initialize the art mode
    artMode->init(seed);

    // For Mood Window, pass weather data
    if (quote.theme == QuoteTheme::SKY) {
        _moodWindow.setWeatherData(_cachedWeather);
    }

    // Set up AmbientPoetryMode with the correct quote for overlay rendering
    _ambientPoetry.setQuoteIndex(_currentQuoteIndex);

    // Generate art commands
    artMode->generate(_engine);
    Serial.printf("[ModeManager] Generated %d draw commands\n", (int)_engine.count());

    // Set up overlay: weather bar + quote text
    static OverlayCtx ctx;
    ctx.weather = &_cachedWeather;
    ctx.timeMgr = &tm;
    ctx.poetry = &_ambientPoetry;
    ctx.showQuote = true;
    ctx.quoteY = QUOTE_AREA_Y;
    ctx.quoteH = QUOTE_AREA_H;
    _engine.setOverlay(overlayCallback, &ctx);

    // Clear and play
    dm.clearScreen();
    delay(300);
    _engine.playback(dm, 30, 180000);  // 30 stages over 3 min

    Serial.printf("[ModeManager] '%s' + quote complete. Next in %d min.\n",
                  artMode->name(), MODE_CYCLE_INTERVAL_MS / 60000);
    Serial.printf("[ModeManager] Free heap after: %d bytes\n", ESP.getFreeHeap());
}

void ModeManager::runPureArt(DisplayManager &dm, TimeManager &tm) {
    // Pick a random art algorithm (no poetry)
    ArtMode* artModes[] = {&_botanist, &_dailyCanvas, &_moodWindow, &_storyCanvas};
    int pick = random(0, 4);
    ArtMode *artMode = artModes[pick];
    _currentModeName = artMode->name();

    uint32_t entropy = esp_random();  // hardware RNG
    uint32_t seed = entropy ^ (_cycleCount * 2654435761UL) ^ (millis() / 1000);

    Serial.printf("[ModeManager] Pure art: '%s' → seed %lu\n",
                  artMode->name(), (unsigned long)seed);
    Serial.printf("[ModeManager] Free heap: %d bytes\n", ESP.getFreeHeap());

    artMode->init(seed);

    // Pass weather for Mood Window
    if (pick == 2) {
        _moodWindow.setWeatherData(_cachedWeather);
    }

    artMode->generate(_engine);
    Serial.printf("[ModeManager] Generated %d draw commands\n", (int)_engine.count());

    // Overlay: weather bar only (no quote)
    static OverlayCtx ctx;
    ctx.weather = &_cachedWeather;
    ctx.timeMgr = &tm;
    ctx.poetry = nullptr;
    ctx.showQuote = false;
    ctx.quoteY = 0;
    ctx.quoteH = 0;

    _engine.setOverlay(overlayCallback, &ctx);

    dm.clearScreen();
    delay(300);
    _engine.playback(dm, 30, 180000);  // 30 stages over 3 min

    // For Botanist — render the label text
    if (pick == 0) {
        int specimenNum = (seed % 900) + 100;
        String dateStr = tm.getFormattedDate();

        dm.display.setPartialWindow(40, SCREEN_H - WEATHER_BAR_H - 45,
                                     SCREEN_W - 80, 38);
        dm.display.firstPage();
        do {
            dm.display.setFont(&FreeMono9pt7b);
            dm.display.setTextColor(GxEPD_BLACK);

            char label[64];
            snprintf(label, sizeof(label), "Specimen #%d", specimenNum);
            int16_t x1, y1;
            uint16_t tw, th;
            dm.display.getTextBounds(label, 0, 0, &x1, &y1, &tw, &th);
            dm.display.setCursor((SCREEN_W - tw) / 2, SCREEN_H - WEATHER_BAR_H - 30);
            dm.display.print(label);

            dm.display.setFont(&FreeSerif9pt7b);
            dm.display.getTextBounds(dateStr.c_str(), 0, 0, &x1, &y1, &tw, &th);
            dm.display.setCursor((SCREEN_W - tw) / 2, SCREEN_H - WEATHER_BAR_H - 14);
            dm.display.print(dateStr.c_str());
        } while (dm.display.nextPage());
    }

    Serial.printf("[ModeManager] '%s' complete. Next in %d min.\n",
                  artMode->name(), MODE_CYCLE_INTERVAL_MS / 60000);
    Serial.printf("[ModeManager] Free heap after: %d bytes\n", ESP.getFreeHeap());
}
