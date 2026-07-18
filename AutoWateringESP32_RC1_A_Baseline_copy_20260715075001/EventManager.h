#pragma once

#include <Arduino.h>
#include "Config.h"

class EventManager
{
public:
    void begin();
    void update(uint8_t pot, float weight);

private:
    struct EventState
    {
        bool initialized = false;
        float referenceWeight = 0.0f;

        bool candidateActive = false;
        float candidateWeight = 0.0f;
        uint32_t candidateSince = 0;
    };

    EventState _state[NUMBER_OF_POTS];
};

extern EventManager Events;