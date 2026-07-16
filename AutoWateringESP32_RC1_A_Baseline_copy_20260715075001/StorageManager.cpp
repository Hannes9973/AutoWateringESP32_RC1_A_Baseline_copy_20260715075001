/*
=========================================================
 AutoWateringESP32
 StorageManager.cpp
=========================================================
*/

#include "StorageManager.h"
#include "ScaleManager.h"
#include "PotManager.h"
//---------------------------------------------------------
// Konstruktor
//---------------------------------------------------------

StorageManager::StorageManager()
{
}

//---------------------------------------------------------
// Preferences öffnen
//---------------------------------------------------------

void StorageManager::begin()
{
    _prefs.begin("AutoWater", false);
}

//---------------------------------------------------------
// Preferences schließen
//---------------------------------------------------------

void StorageManager::end()
{
    _prefs.end();
}

//---------------------------------------------------------
// Key für Offset
//---------------------------------------------------------

String StorageManager::keyOffset(uint8_t pot) const
{
    return "off" + String(pot);
}

//---------------------------------------------------------
// Key für Kalibrierung
//---------------------------------------------------------

String StorageManager::keyCalibration(uint8_t pot) const
{
    return "cal" + String(pot);
}

//---------------------------------------------------------
// Waage speichern
//---------------------------------------------------------

bool StorageManager::saveScale(uint8_t pot,
                               const ScaleManager& scale)
{
    if (pot >= NUMBER_OF_POTS)
        return false;

    _prefs.putLong(
        keyOffset(pot).c_str(),
        scale.getOffset(pot));

    _prefs.putFloat(
        keyCalibration(pot).c_str(),
        scale.getCalibration(pot));

    return true;
}

//---------------------------------------------------------
// Waage laden
//---------------------------------------------------------

bool StorageManager::loadScale(uint8_t pot,
                               ScaleManager& scale)
                               
{
    if (pot >= NUMBER_OF_POTS)
        return false;

    long offset =
        _prefs.getLong(
            keyOffset(pot).c_str(),
            0);

    float calibration =
        _prefs.getFloat(
            keyCalibration(pot).c_str(),
            DEFAULT_CALIBRATION_FACTOR);

    scale.setOffset(
        pot,
        offset);

    scale.setCalibration(
        pot,
        calibration);

    return true;
    
}
void StorageManager::savePotConfig(uint8_t potIndex, const PotManager& pot)
{
    String key = "pot" + String(potIndex);

    _prefs.putFloat((key + "_start").c_str(), pot.getStartWeight());
    _prefs.putFloat((key + "_target").c_str(), pot.getTargetWeight());
    _prefs.putBool((key + "_auto").c_str(), pot.getAutoMode());
}

void StorageManager::loadPotConfig(uint8_t potIndex, PotManager& pot)
{
    String key = "pot" + String(potIndex);

    pot.setStartWeight(
        _prefs.getFloat((key + "_start").c_str(),
                        pot.getStartWeight()));

    pot.setTargetWeight(
        _prefs.getFloat((key + "_target").c_str(),
                        pot.getTargetWeight()));

    pot.setAutoMode(
        _prefs.getBool((key + "_auto").c_str(),
                       pot.getAutoMode()));
}