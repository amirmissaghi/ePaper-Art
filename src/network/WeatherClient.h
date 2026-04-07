// WeatherClient.h — Fetches weather from Open-Meteo API (no API key required)
#pragma once

#include <Arduino.h>
#include "config.h"

struct WeatherData {
    float temperature;     // °C
    float humidity;        // %
    float windSpeed;       // km/h
    int weatherCode;       // WMO weather code
    bool isDay;
    bool valid;            // false if fetch failed
};

class WeatherClient {
public:
    WeatherData fetch();

    // Interpret WMO weather code
    static const char* weatherCodeToString(int code);
    static bool isRainy(int code);
    static bool isSnowy(int code);
    static bool isCloudy(int code);
    static bool isClear(int code);
};
