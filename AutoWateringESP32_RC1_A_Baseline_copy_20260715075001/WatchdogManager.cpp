#include "WatchdogManager.h"

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "WatchdogManager.h"

WatchdogManager Watchdog;

void WatchdogManager::begin()
{
    // Der ESP32-Core initialisiert den Task-Watchdog bereits.
    // Deshalb hier KEIN esp_task_wdt_init() aufrufen.

    esp_err_t err = esp_task_wdt_add(NULL);

    if (err == ESP_OK)
    {
        Serial.println("Watchdog: Loop-Task registriert");
    }
    else if (err == ESP_ERR_INVALID_STATE)
    {
        Serial.println("Watchdog: Loop-Task bereits registriert");
    }
    else
    {
        Serial.printf("Watchdog: Add Fehler %d\n", err);
    }

    Serial.println("Watchdog gestartet");
}

void WatchdogManager::update()
{
    esp_task_wdt_reset();
}