#include "WiFiManager.h"

#include <WiFi.h>

#include "Config.h"
#include "EventLogger.h"

WiFiManager WiFiMonitor;

void WiFiManager::begin()
{
    _wasConnected =
        (WiFi.status() == WL_CONNECTED);
}

void WiFiManager::update()
{
    if(millis() - _lastCheck < 10000)
        return;

    _lastCheck = millis();

    bool connected =
        (WiFi.status() == WL_CONNECTED);

    if(!connected && _wasConnected)
    {
        Logger.add("WLAN verloren");

        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }

    if(connected && !_wasConnected)
    {
        Logger.add("WLAN wieder verbunden");
    }

    _wasConnected = connected;
}