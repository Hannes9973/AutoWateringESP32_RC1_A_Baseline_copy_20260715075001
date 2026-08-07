#include "OTAManager.h"

#include <WiFi.h>
#include <ArduinoOTA.h>

void OTAManager::begin()
{
    ArduinoOTA.setHostname("AutoWateringESP32");

    ArduinoOTA
        .onStart([]()
        {
            Serial.println();
            Serial.println("===== OTA START =====");
        });

    ArduinoOTA
        .onEnd([]()
        {
            Serial.println();
            Serial.println("===== OTA ENDE =====");
        });

    ArduinoOTA
        .onProgress([](unsigned int progress,
                       unsigned int total)
        {
            Serial.printf("OTA %u %%\r",
                          (progress * 100) / total);
        });

    ArduinoOTA
        .onError([](ota_error_t error)
        {
            Serial.printf("OTA Fehler %u\n", error);
        });

    ArduinoOTA.begin();
ArduinoOTA.setRebootOnSuccess(true);
    Serial.println("Arduino OTA bereit");
}

void OTAManager::update()
{
    ArduinoOTA.handle();
}