// TimeManager.cpp
#include "network/TimeManager.h"
#include <WiFi.h>

static const char* MONTH_NAMES[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

void TimeManager::init() {
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org", TIMEZONE_OFFSET_SEC, 3600000);
    timeClient->begin();
    timeClient->update();
    Serial.printf("[Time] NTP synced: %s\n", timeClient->getFormattedTime().c_str());
}

void TimeManager::update() {
    if (timeClient && WiFi.status() == WL_CONNECTED) {
        timeClient->update();
    }
}

int TimeManager::getHour() {
    return timeClient ? timeClient->getHours() : 0;
}

int TimeManager::getMinute() {
    return timeClient ? timeClient->getMinutes() : 0;
}

unsigned long TimeManager::getEpoch() {
    return timeClient ? timeClient->getEpochTime() : 0;
}

int TimeManager::getDay() {
    unsigned long epoch = getEpoch();
    // Extract day from epoch using gmtime-like calculation
    time_t t = (time_t)epoch;
    struct tm *tm = localtime(&t);
    return tm ? tm->tm_mday : 1;
}

int TimeManager::getMonth() {
    unsigned long epoch = getEpoch();
    time_t t = (time_t)epoch;
    struct tm *tm = localtime(&t);
    return tm ? (tm->tm_mon + 1) : 1;  // 1-based
}

int TimeManager::getYear() {
    unsigned long epoch = getEpoch();
    time_t t = (time_t)epoch;
    struct tm *tm = localtime(&t);
    return tm ? (tm->tm_year + 1900) : 2026;
}

uint32_t TimeManager::getDailySeed(int modeIndex) {
    // Combine year, month, day, and mode index into a deterministic seed
    return (uint32_t)(getYear() * 10000 + getMonth() * 100 + getDay()) * 7 + modeIndex * 31;
}

String TimeManager::getFormattedDate() {
    int m = getMonth();
    int d = getDay();
    int y = getYear();
    if (m < 1 || m > 12) m = 1;
    char buf[64];
    snprintf(buf, sizeof(buf), "%s %d, %d", MONTH_NAMES[m - 1], d, y);
    return String(buf);
}

String TimeManager::getTimeString() {
    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", getHour(), getMinute());
    return String(buf);
}
