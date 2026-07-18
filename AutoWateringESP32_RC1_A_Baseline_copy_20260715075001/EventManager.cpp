#include "EventManager.h"

EventManager Events;

void EventManager::begin()
{
}

void EventManager::update(uint8_t pot, float weight)
{
    EventState &s = _state[pot];

    if(!s.initialized)
    {
        s.initialized = true;
        s.lastWeight = weight;
        return;
    }

    float diff = weight - s.lastWeight;

    if(!s.candidateActive)
    {
        if(diff >= WATERING_EVENT_THRESHOLD)
        {
            s.candidateActive = true;
            s.candidateWeight = weight;
            s.candidateSince = millis();

            Serial.printf(
                "[EVENT] Kandidat Topf %u (+%.1f g)\n",
                pot + 1,
                diff
            );
        }

        s.lastWeight = weight;
        return;
    }

    if(weight < s.candidateWeight - WATERING_STABLE_DELTA)
    {
        Serial.printf(
            "[EVENT] Kandidat verworfen Topf %u\n",
            pot + 1
        );

        s.candidateActive = false;
        s.lastWeight = weight;
        return;
    }

    if(millis() - s.candidateSince >= WATERING_CONFIRM_TIME)
    {
        Serial.printf(
            "[EVENT] Topf %u gegossen\n",
            pot + 1
        );

        s.candidateActive = false;
    }

    s.lastWeight = weight;
}