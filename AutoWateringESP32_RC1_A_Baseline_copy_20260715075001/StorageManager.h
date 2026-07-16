#pragma once

#include <Arduino.h>
#include <Preferences.h>

class ScaleManager;
class PotManager;
class StorageManager
{
public:

    StorageManager();

    //--------------------------------------------------
    // Initialisierung
    //--------------------------------------------------

    void begin();
    void end();
void savePotConfig(uint8_t potIndex, const PotManager& pot);
void loadPotConfig(uint8_t potIndex, PotManager& pot);
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