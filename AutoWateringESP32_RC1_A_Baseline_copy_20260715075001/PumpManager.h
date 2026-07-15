#pragma once

#include <Arduino.h>
#include "Config.h"

class PumpManager
{
public:

    PumpManager();

    //----------------------------------------------------
    // Initialisierung
    //----------------------------------------------------

    void begin();

    //----------------------------------------------------
    // zyklischer Aufruf
    //----------------------------------------------------

    void update();

    //----------------------------------------------------
    // Steuerung
    //----------------------------------------------------

    void on(uint8_t pump);

    void off(uint8_t pump);

    void toggle(uint8_t pump);

    void stopAll();

    //----------------------------------------------------
    // Status
    //----------------------------------------------------

    bool isRunning(uint8_t pump) const;

private:

    bool _running[NUMBER_OF_POTS];

    unsigned long _startTime[NUMBER_OF_POTS];
};