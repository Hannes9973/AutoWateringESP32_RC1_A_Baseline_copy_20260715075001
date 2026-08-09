#include "TimeManager.h"

#include <WiFi.h>
#include <time.h>

TimeManager Time;

void TimeManager::begin()
{
    configTzTime(
    "CET-1CEST,M3.5.0/2,M10.5.0/3",
    "pool.ntp.org",
    "time.nist.gov");

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
//------------------------------------------------------
// Uhrzeit HH:MM:SS
//------------------------------------------------------

String TimeManager::getTimeString()
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
        return "--:--:--";

    char buffer[16];

    strftime(
        buffer,
        sizeof(buffer),
        "%H:%M:%S",
        &timeinfo);

    return String(buffer);
}

//------------------------------------------------------
// Datum + Uhrzeit
//------------------------------------------------------

String TimeManager::getDateTimeString()
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
        return "----";

    char buffer[32];

    strftime(
        buffer,
        sizeof(buffer),
        "%d.%m.%Y %H:%M:%S",
        &timeinfo);

    return String(buffer);
}