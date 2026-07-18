#include "TimeManager.h"

#include <WiFi.h>
#include <time.h>

TimeManager Time;

void TimeManager::begin()
{
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    struct tm timeinfo;

    _valid = getLocalTime(&timeinfo, 10000);
}

bool TimeManager::isValid()
{
    return _valid;
}

uint32_t TimeManager::now()
{
    time_t now;
    time(&now);

    return (uint32_t)now;
}