#include "ResetManager.h"

#include <esp_system.h>

ResetManager Reset;

void ResetManager::begin()
{
    esp_reset_reason_t reason = esp_reset_reason();

    switch(reason)
    {
        case ESP_RST_POWERON:
            _reason = "Power-On";
            break;

        case ESP_RST_SW:
            _reason = "Software Reset";
            break;

        case ESP_RST_PANIC:
            _reason = "Guru Meditation";
            break;

        case ESP_RST_INT_WDT:
            _reason = "Interrupt Watchdog";
            break;

        case ESP_RST_TASK_WDT:
            _reason = "Task Watchdog";
            break;

        case ESP_RST_WDT:
            _reason = "Watchdog";
            break;

        case ESP_RST_BROWNOUT:
            _reason = "Brownout";
            break;

        default:
            _reason = "Unbekannt";
            break;
    }
}

String ResetManager::getReason() const
{
    return _reason;
}