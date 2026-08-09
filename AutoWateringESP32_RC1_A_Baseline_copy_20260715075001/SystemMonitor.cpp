#include "SystemMonitor.h"

#include <WiFi.h>

void SystemMonitor::begin()
{
    Serial.println("SystemMonitor gestartet");
}

void SystemMonitor::update()
{
    //------------------------------------------------------
// WLAN überwachen
//------------------------------------------------------

if(WiFi.status() != WL_CONNECTED)
{
    if(_wifiWasConnected)
    {
        Serial.println();
        Serial.println("WLAN getrennt!");

        _wifiWasConnected = false;
    }

    if(millis() - _lastReconnect > 30000)
    {
        _lastReconnect = millis();

        _reconnectCounter++;

        Serial.println("Versuche WLAN-Reconnect...");

        WiFi.disconnect();

        delay(100);

        WiFi.reconnect();
    }
}
else
{
    if(!_wifiWasConnected)
    {
        Serial.println("WLAN wieder verbunden.");

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        _wifiWasConnected = true;
    }
}
    if(millis() - _lastPrint < 60000)
        return;

    _lastPrint = millis();

    Serial.println("--------------------------------");

    Serial.print("Heap: ");
    Serial.println(getFreeHeap());

    Serial.print("Min Heap: ");
    Serial.println(getMinFreeHeap());

    Serial.print("RSSI: ");
    Serial.println(getRSSI());

    Serial.print("IP: ");
    Serial.println(getIPAddress());

    Serial.print("Uptime: ");
    Serial.println(getUptimeString());

    Serial.println("--------------------------------");
}

uint32_t SystemMonitor::getFreeHeap() const
{
    return ESP.getFreeHeap();
}

uint32_t SystemMonitor::getMinFreeHeap() const
{
    return ESP.getMinFreeHeap();
}

int SystemMonitor::getRSSI() const
{
    return WiFi.RSSI();
}

String SystemMonitor::getIPAddress() const
{
    return WiFi.localIP().toString();
}

uint32_t SystemMonitor::getUptimeSeconds() const
{
    return millis()/1000;
}

String SystemMonitor::getUptimeString() const
{
    uint32_t s = millis()/1000;

    uint32_t d = s/86400;

    s %= 86400;

    uint32_t h = s/3600;

    s %= 3600;

    uint32_t m = s/60;

    s %= 60;

    return String(d)+"d "
         + String(h)+"h "
         + String(m)+"m "
         + String(s)+"s";
}

String SystemMonitor::getResetReason() const
{
    return String(esp_reset_reason());
}
uint32_t SystemMonitor::getReconnectCounter() const
{
    return _reconnectCounter;
}