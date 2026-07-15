#pragma once

#include <Arduino.h>
#include <Preferences.h>

class ScaleManager;

class StorageManager
{
public:

    StorageManager();

    //--------------------------------------------------
    // Initialisierung
    //--------------------------------------------------

    void begin();
    void end();

    //--------------------------------------------------
    // Waagendaten
    //--------------------------------------------------

    bool saveScale(uint8_t pot,
                   const ScaleManager& scale);

    bool loadScale(uint8_t pot,
                   ScaleManager& scale);

private:

    Preferences _prefs;

    String keyOffset(uint8_t pot) const;

    String keyCalibration(uint8_t pot) const;
};