#include "WateringLogManager.h"

WateringLogManager WateringLog;

void WateringLogManager::begin()
{
}

String WateringLogManager::getFilename(uint8_t pot) const
{
    return "/watering" + String(pot) + ".bin";
}

bool WateringLogManager::add(uint8_t pot,
                             uint32_t timestamp,
                             float amount)
{
    WateringPoint point;

    point.timestamp = timestamp;
    point.amount = amount;

    File file = LittleFS.open(getFilename(pot), FILE_APPEND);

    if(!file)
    {
        return false;
    }

    file.write(
        (const uint8_t*)&point,
        sizeof(point)
    );

    file.close();

    return true;
}

bool WateringLogManager::load(uint8_t pot,
                              std::vector<WateringPoint>& data)
{
    data.clear();

    File file = LittleFS.open(getFilename(pot), FILE_READ);

    if(!file)
    {
        return false;
    }

    WateringPoint point;

    while(file.available())
    {
        if(file.read((uint8_t*)&point, sizeof(point)) != sizeof(point))
        {
            break;
        }

        data.push_back(point);
    }

    file.close();

    return true;
}

void WateringLogManager::clear(uint8_t pot)
{
    LittleFS.remove(getFilename(pot));
}