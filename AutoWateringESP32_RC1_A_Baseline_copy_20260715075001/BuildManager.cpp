#include "BuildManager.h"

#include <WiFi.h>

#include "Config.h"

String BuildManager::firmwareName()
{
    return FW_NAME;
}

String BuildManager::firmwareVersion()
{
    return FW_VERSION;
}

String BuildManager::firmwareBranch()
{
    return FW_BRANCH;
}

String BuildManager::buildDate()
{
    return BUILD_DATE;
}

String BuildManager::buildTime()
{
    return BUILD_TIME;
}

String BuildManager::ipAddress()
{
    return WiFi.localIP().toString();
}

String BuildManager::hostName()
{
    return WiFi.getHostname();
}

uint32_t BuildManager::freeHeap()
{
    return ESP.getFreeHeap();
}

String BuildManager::uptime()
{
    uint32_t sec = millis() / 1000;

    uint32_t days = sec / 86400;
    sec %= 86400;

    uint8_t hours = sec / 3600;
    sec %= 3600;

    uint8_t minutes = sec / 60;
    uint8_t seconds = sec % 60;

    char txt[32];

    sprintf(txt,
            "%lu d %02u:%02u:%02u",
            days,
            hours,
            minutes,
            seconds);

    return String(txt);
}