#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <Arduino.h>

class SystemMonitor
{
public:

    void begin();

    void update();

    uint32_t getUptimeSeconds() const;

    String getUptimeString() const;

    uint32_t getFreeHeap() const;

    uint32_t getMinFreeHeap() const;

    int getRSSI() const;

    String getIPAddress() const;

    String getResetReason() const;

    uint32_t getReconnectCounter() const;
    
private:

    uint32_t _lastPrint = 0;
bool _wifiWasConnected = true;

uint32_t _lastReconnect = 0;

uint32_t _reconnectCounter = 0;
};

#endif