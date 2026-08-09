#pragma once

#include <Arduino.h>

class WiFiManager
{
public:

    void begin();

    void update();

private:

    uint32_t _lastCheck = 0;
    bool _wasConnected = true;
};

extern WiFiManager WiFiMonitor;