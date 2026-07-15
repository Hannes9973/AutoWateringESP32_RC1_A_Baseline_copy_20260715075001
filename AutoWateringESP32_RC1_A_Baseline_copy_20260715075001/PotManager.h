#pragma once

#include <Arduino.h>
#include "Types.h"

class ScaleManager;
class PumpManager;

class PotManager
{
public:
    PotManager();

    void begin(uint8_t potNumber,
               ScaleManager* scale,
               PumpManager* pump);

    void update();

    void setAutoMode(bool enable);
    bool getAutoMode() const;

    void setStartWeight(float g);
    void setTargetWeight(float g);

    float getStartWeight() const;
    float getTargetWeight() const;
    float getWeight() const;

    PotState getState() const;
    const char* getStateName() const;
    const PotStatus& getStatus() const;

private:
    uint8_t _pot;
    ScaleManager* _scale;
    PumpManager* _pump;
    PotStatus _status;
};
