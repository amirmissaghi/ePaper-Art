// WeatherClient.cpp
#include "network/WeatherClient.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

WeatherData WeatherClient::fetch() {
    WeatherData data = {0, 0, 0, 0, true, false};

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[Weather] No WiFi, skipping fetch");
        return data;
    }

    HTTPClient http;
    char url[256];
    snprintf(url, sizeof(url),
        "https://api.open-meteo.com/v1/forecast?"
        "latitude=%.2f&longitude=%.2f"
        "&current=temperature_2m,relative_humidity_2m,wind_speed_10m,weather_code,is_day",
        LOCATION_LAT, LOCATION_LON);

    Serial.printf("[Weather] Fetching: %s\n", url);
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            JsonObject current = doc["current"];
            data.temperature = current["temperature_2m"] | 0.0f;
            data.humidity = current["relative_humidity_2m"] | 0.0f;
            data.windSpeed = current["wind_speed_10m"] | 0.0f;
            data.weatherCode = current["weather_code"] | 0;
            data.isDay = current["is_day"] | 1;
            data.valid = true;
            Serial.printf("[Weather] Temp=%.1f°C Humid=%.0f%% Wind=%.1fkm/h Code=%d\n",
                          data.temperature, data.humidity, data.windSpeed, data.weatherCode);
        } else {
            Serial.printf("[Weather] JSON parse error: %s\n", error.c_str());
        }
    } else {
        Serial.printf("[Weather] HTTP error: %d\n", httpCode);
    }

    http.end();
    return data;
}

const char* WeatherClient::weatherCodeToString(int code) {
    if (code == 0) return "Clear";
    if (code <= 3) return "Cloudy";
    if (code <= 49) return "Fog";
    if (code <= 59) return "Drizzle";
    if (code <= 69) return "Rain";
    if (code <= 79) return "Snow";
    if (code <= 84) return "Showers";
    if (code <= 86) return "Snow Showers";
    if (code <= 99) return "Thunderstorm";
    return "Unknown";
}

bool WeatherClient::isRainy(int code) { return (code >= 51 && code <= 67) || (code >= 80 && code <= 84); }
bool WeatherClient::isSnowy(int code) { return (code >= 70 && code <= 77) || (code >= 85 && code <= 86); }
bool WeatherClient::isCloudy(int code) { return code >= 1 && code <= 3; }
bool WeatherClient::isClear(int code) { return code == 0; }
