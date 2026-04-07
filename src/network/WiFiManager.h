// WiFiManager.h — WiFi with duty cycling for power savings
#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

class WiFiMgr {
public:
    // Connect to WiFi (blocking, with timeout)
    bool connect(int timeoutMs = 10000);

    // Check connection status
    bool isConnected();

    // Ensure WiFi is connected (reconnect if needed)
    bool ensureConnected();

    // Turn WiFi off completely to save power (~80-150mA savings)
    void sleep();

    // Wake WiFi up and reconnect
    bool wake();

    String localIP();
};
