// main.cpp — ePaper Art Device
// ESP32 + Waveshare 4.2" e-Paper V2.2
//
// Power strategy: WiFi is turned OFF during art drawing (~5 min)
// and only turned ON briefly to fetch weather data (~20 min intervals).
// This saves ~80-150mA during drawing vs. always-on WiFi.

#include <Arduino.h>
#include "config.h"
#include "display/DisplayManager.h"
#include "network/WiFiManager.h"
#include "network/TimeManager.h"
#include "modes/ModeManager.h"

DisplayManager displayMgr;
WiFiMgr wifiMgr;
TimeManager timeMgr;
ModeManager modeMgr;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("╔══════════════════════════════════╗");
    Serial.println("║       ePaper Art Device v1.0      ║");
    Serial.println("╚══════════════════════════════════╝");
    Serial.println();

    // Initialize display
    Serial.println("[Setup] Initializing display...");
    displayMgr.init();
    displayMgr.clearScreen();

    // Show boot message
    displayMgr.display.setFullWindow();
    displayMgr.display.firstPage();
    do {
        displayMgr.display.fillScreen(GxEPD_WHITE);
        displayMgr.display.setTextColor(GxEPD_BLACK);
        displayMgr.display.setTextSize(1);
        displayMgr.display.setCursor(100, 190);
        displayMgr.display.print("ePaper Art");
        displayMgr.display.setCursor(80, 210);
        displayMgr.display.print("Connecting...");
    } while (displayMgr.display.nextPage());

    // Connect WiFi and sync time
    Serial.println("[Setup] Connecting WiFi...");
    bool wifiOk = wifiMgr.connect(15000);

    if (wifiOk) {
        Serial.println("[Setup] Syncing time...");
        timeMgr.init();
    } else {
        Serial.println("[Setup] WiFi failed — running offline");
    }

    Serial.printf("[Setup] Free heap: %d bytes\n", ESP.getFreeHeap());

    // Start art modes (this will fetch weather, then turn off WiFi)
    modeMgr.init(displayMgr, timeMgr, wifiMgr);

    // Turn off WiFi after initial setup — art doesn't need it
    wifiMgr.sleep();

    Serial.println("[Setup] Ready! WiFi sleeping until next weather fetch.");
}

void loop() {
    // Check if it's time to switch modes
    modeMgr.update(displayMgr, timeMgr);

    delay(1000);
}
