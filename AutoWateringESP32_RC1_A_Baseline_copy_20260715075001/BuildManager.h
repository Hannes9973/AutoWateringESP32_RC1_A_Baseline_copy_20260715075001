#ifndef BUILD_MANAGER_H
#define BUILD_MANAGER_H

#include <Arduino.h>

class BuildManager
{
public:

    static String firmwareName();
    static String firmwareVersion();
    static String firmwareBranch();

    static String buildDate();
    static String buildTime();

    static String ipAddress();
    static String hostName();

    static uint32_t freeHeap();

    static String uptime();
};

#endif