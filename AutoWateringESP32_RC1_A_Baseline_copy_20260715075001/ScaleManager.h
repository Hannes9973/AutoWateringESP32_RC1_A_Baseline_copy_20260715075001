#pragma once

#include <Arduino.h>
#include <HX711.h>

#include "Config.h"

//=========================================================
// ScaleManager
//=========================================================

class ScaleManager
{
public:

    ScaleManager();

    //-----------------------------------------------------
    // Initialisierung
    //-----------------------------------------------------

    void begin();
    void update();

    //-----------------------------------------------------
    // Status
    //-----------------------------------------------------

    bool isReady(uint8_t pot) const;

    //-----------------------------------------------------
    // Messwerte
    //-----------------------------------------------------

    long getRaw(uint8_t pot) const;

    float getWeight(uint8_t pot) const;

    //-----------------------------------------------------
    // Tara / Kalibrierung
    //-----------------------------------------------------

    void tare(uint8_t pot);

    void setOffset(uint8_t pot, long value);
    long getOffset(uint8_t pot) const;

    void setCalibration(uint8_t pot, float factor);
    float getCalibration(uint8_t pot) const;

private:

    //-----------------------------------------------------
    // Hardware
    //-----------------------------------------------------

    HX711 _scale[NUMBER_OF_POTS];

    //-----------------------------------------------------
    // Messwerte
    //-----------------------------------------------------

    long _raw[NUMBER_OF_POTS];

    long _offset[NUMBER_OF_POTS];

    float _weight[NUMBER_OF_POTS];

    float _calibration[NUMBER_OF_POTS];

    bool _ready[NUMBER_OF_POTS];

    //-----------------------------------------------------
    // interne Funktionen
    //-----------------------------------------------------

    long readAverage(uint8_t pot,
                     uint8_t samples = 10);
};