#pragma once

#include "Config.h"
#include <Arduino.h>

struct WaterEvent
{
    uint32_t timestamp;
    uint8_t pot;
    float amount;
};

class EventManager
{
public:
    void begin();
    void update(uint8_t pot, float weight);

private:
    float _lastWeight[NUMBER_OF_POTS];
    bool _initialized[NUMBER_OF_POTS];
};

extern EventManager Events;