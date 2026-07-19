#include "EventManager.h"
#include "WateringLogManager.h"

EventManager Events;

void EventManager::begin()
{
}

void EventManager::update(uint8_t pot, float weight)
{
    EventState &s = _state[pot];

    // Erste Messung
 // Erst initialisieren, wenn die Waage einen plausiblen Wert liefert
if (!s.initialized)
{
    if (weight < 10.0f)
    {
        return;
    }

    s.initialized = true;
    s.referenceWeight = weight;

    Serial.printf(
        "[EVENT] Referenz Topf %u = %.1f g\n",
        pot + 1,
        weight
    );

    return;
}

    //------------------------------------------------------
    // Kein Kandidat aktiv
    //------------------------------------------------------
    if (!s.candidateActive)
    {
        float diff = weight - s.referenceWeight;

        if (diff >= WATERING_EVENT_THRESHOLD)
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

        return;
    }

    //------------------------------------------------------
    // Kandidat aktiv
    //------------------------------------------------------

    // Neues Maximum gefunden
    if (weight > s.candidateWeight)
    {
        s.candidateWeight = weight;

        // Timer neu starten
        s.candidateSince = millis();
    }

    // Nur bei deutlichem Gewichtsverlust verwerfen
    if (weight < (s.candidateWeight - WATERING_STABLE_DELTA))
    {
        Serial.printf(
            "[EVENT] Kandidat verworfen Topf %u\n",
            pot + 1
        );

        s.candidateActive = false;
        return;
    }

    // Noch nicht lange genug stabil
    if (millis() - s.candidateSince < WATERING_CONFIRM_TIME)
    {
        return;
    }

    //------------------------------------------------------
// Gießvorgang bestätigt
//------------------------------------------------------

float amount = s.candidateWeight - s.referenceWeight;

Serial.printf(
    "[EVENT] Topf %u gegossen (+%.1f g)\n",
    pot + 1,
    amount
);
WateringLog.add(
    pot,
    Time.now(),
    amount
);
s.referenceWeight = s.candidateWeight;
s.candidateActive = false;
}