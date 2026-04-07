// WiFiManager.cpp — WiFi with duty cycling
#include "network/WiFiManager.h"

bool WiFiMgr::connect(int timeoutMs) {
    if (WiFi.status() == WL_CONNECTED) return true;

    Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < (unsigned long)timeoutMs) {
        delay(250);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
        return true;
    } else {
        Serial.println("\n[WiFi] Connection failed!");
        return false;
    }
}

bool WiFiMgr::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiMgr::ensureConnected() {
    if (!isConnected()) {
        Serial.println("[WiFi] Reconnecting...");
        return connect();
    }
    return true;
}

void WiFiMgr::sleep() {
    Serial.println("[WiFi] Turning off radio (power save)");
    WiFi.disconnect(true);  // Disconnect and turn off radio
    WiFi.mode(WIFI_OFF);
    delay(10);
}

bool WiFiMgr::wake() {
    Serial.println("[WiFi] Waking up radio...");
    WiFi.mode(WIFI_STA);
    return connect(10000);
}

String WiFiMgr::localIP() {
    return WiFi.localIP().toString();
}
