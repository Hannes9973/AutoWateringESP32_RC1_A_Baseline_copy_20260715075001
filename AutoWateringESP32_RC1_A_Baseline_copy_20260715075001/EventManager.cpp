#include "EventManager.h"
#include "TimeManager.h"

EventManager Events;

void EventManager::begin()
{
    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        _initialized[i] = false;
        _lastWeight[i] = 0.0f;
    }
}

void EventManager::update(uint8_t pot, float weight)
{Serial.printf("EventManager: Topf %u Gewicht %.1f g\n", pot + 1, weight);
    if(!_initialized[pot])
    {
        _initialized[pot] = true;
        _lastWeight[pot] = weight;
        return;
    }

    float diff = weight - _lastWeight[pot];

    if(diff > 20.0f)
    {
        Serial.printf(
            "[EVENT] Topf %u gegossen (+%.1f g)\n",
            pot + 1,
            diff
        );
    }

    _lastWeight[pot] = weight;
}