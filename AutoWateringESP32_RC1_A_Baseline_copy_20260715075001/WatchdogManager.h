#pragma once

#include <Arduino.h>

class WatchdogManager
{
public:

    void begin();

    void update();
};

extern WatchdogManager Watchdog;