#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <vector>

struct WateringPoint
{
    uint32_t timestamp;
    float amount;
};

class WateringLogManager
{
public:
    void begin();

    bool add(uint8_t pot,
             uint32_t timestamp,
             float amount);

    bool load(uint8_t pot,
              std::vector<WateringPoint>& data);

    void clear(uint8_t pot);

private:
    String getFilename(uint8_t pot) const;
};

extern WateringLogManager WateringLog;