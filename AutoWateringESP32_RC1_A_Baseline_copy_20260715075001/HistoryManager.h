#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <vector>
#include "TimeManager.h"

struct HistoryPoint
{
    uint32_t timestamp;
    float weight;
};

class HistoryManager
{
public:

    void begin();

    bool append(uint8_t pot, float weight);

    bool load(uint8_t pot, std::vector<HistoryPoint>& history);

    void clear(uint8_t pot);

private:

    String fileName(uint8_t pot);
    static constexpr size_t MAX_HISTORY_POINTS = 1000;


};