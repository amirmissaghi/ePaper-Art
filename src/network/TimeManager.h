// TimeManager.h — NTP time sync and date-based seed generation
#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "config.h"

class TimeManager {
public:
    void init();
    void update();

    int getHour();
    int getMinute();
    int getDay();
    int getMonth();
    int getYear();

    // Generate a deterministic seed from the current date + mode index
    // Same date + mode = same artwork; different day = different artwork
    uint32_t getDailySeed(int modeIndex = 0);

    // Get epoch seconds
    unsigned long getEpoch();

    String getFormattedDate();   // "April 6, 2026"
    String getTimeString();     // "10:42"

private:
    WiFiUDP ntpUDP;
    NTPClient *timeClient = nullptr;
};
