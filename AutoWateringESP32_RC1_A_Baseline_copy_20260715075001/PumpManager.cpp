/*
=========================================================
 AutoWateringESP32
 PumpManager.cpp
=========================================================
*/

#include "PumpManager.h"

//---------------------------------------------------------
// Konstruktor
//---------------------------------------------------------

PumpManager::PumpManager()
{
    for (uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        _running[i] = false;
        _startTime[i] = 0;
    }
}

//---------------------------------------------------------
// Initialisierung
//---------------------------------------------------------

void PumpManager::begin()
{
    for (uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        pinMode(PUMP_PIN[i], OUTPUT);

        digitalWrite(PUMP_PIN[i], LOW);

        _running[i] = false;
    }
}

//---------------------------------------------------------
// Update
//---------------------------------------------------------

void PumpManager::update()
{
    unsigned long now = millis();

    for (uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        if (!_running[i])
            continue;

        if (now - _startTime[i] >= MAX_PUMP_RUNTIME)
        {
            off(i);

            Serial.print("Pumpe ");
            Serial.print(i + 1);
            Serial.println(" Timeout");
        }
    }
}

//---------------------------------------------------------
// EIN
//---------------------------------------------------------

void PumpManager::on(uint8_t pump)
{
    if (pump >= NUMBER_OF_POTS)
        return;

    digitalWrite(PUMP_PIN[pump], HIGH);

    _running[pump] = true;

    _startTime[pump] = millis();
}

//---------------------------------------------------------
// AUS
//---------------------------------------------------------

void PumpManager::off(uint8_t pump)
{
    if (pump >= NUMBER_OF_POTS)
        return;

    digitalWrite(PUMP_PIN[pump], LOW);

    _running[pump] = false;
}

//---------------------------------------------------------
// Umschalten
//---------------------------------------------------------

void PumpManager::toggle(uint8_t pump)
{
    if (pump >= NUMBER_OF_POTS)
        return;

    if (_running[pump])
        off(pump);
    else
        on(pump);
}

//---------------------------------------------------------
// Alle AUS
//---------------------------------------------------------

void PumpManager::stopAll()
{
    for (uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        off(i);
    }
}

//---------------------------------------------------------
// Status
//---------------------------------------------------------

bool PumpManager::isRunning(uint8_t pump) const
{
    if (pump >= NUMBER_OF_POTS)
        return false;

    return _running[pump];
}