#pragma once

/*
=========================================================
 AutoWateringESP32
 Config.h

 Zentrale Projektkonfiguration
=========================================================
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//=========================================================
// Allgemein
//=========================================================

constexpr uint32_t SERIAL_BAUD = 115200;

constexpr uint8_t NUMBER_OF_POTS = 3;
//=========================================================
// Bewässerung
//=========================================================

constexpr float DEFAULT_START_WEIGHT = 950.0f;

constexpr float DEFAULT_TARGET_WEIGHT = 1100.0f;

constexpr float MIN_WEIGHT_GAIN = 5.0f;          // mindestens 5 g Zunahme
constexpr uint32_t TANK_CHECK_TIME = 5000UL;     // nach 5 Sekunden prüfen

// Ereigniserkennung
constexpr float WATERING_EVENT_THRESHOLD = 20.0f;     // mindestens 20 g Gewichtszunahme
constexpr float WATERING_STABLE_DELTA = 2.0f;         // maximal ±2 g Schwankung
constexpr uint32_t WATERING_CONFIRM_TIME = 3000UL;    // 3 Sekunden stabil

constexpr float MIN_ALLOWED_WEIGHT = 0.0f;
constexpr float MAX_ALLOWED_WEIGHT = 5000.0f;
//=========================================================
// WLAN
//=========================================================

constexpr bool WIFI_ENABLED = true;

constexpr char WIFI_SSID[] = "Galaxy S1091db";

constexpr char WIFI_PASSWORD[] = "ywdb0133";

//=========================================================
// Logging
//=========================================================

enum class LogLevel : uint8_t
{
    NONE = 0,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

constexpr LogLevel DEFAULT_LOG_LEVEL = LogLevel::INFO;

//=========================================================
// Diagnose
//=========================================================

constexpr uint32_t STATUS_INTERVAL = 1000UL;

//=========================================================
// HX711
//=========================================================

constexpr uint8_t HX711_DT[NUMBER_OF_POTS] =
{
    4,
    22,
    18
};

constexpr uint8_t HX711_SCK[NUMBER_OF_POTS] =
{
    5,
    21,
    19
};

//=========================================================
// Kalibrierung
//=========================================================

constexpr float DEFAULT_CALIBRATION_FACTOR = 110.63f;

constexpr uint8_t SCALE_AVERAGE_SAMPLES = 10;

constexpr uint8_t SCALE_TARE_SAMPLES = 20;

//=========================================================
// Pumpen
//=========================================================

constexpr uint8_t PUMP_PIN[NUMBER_OF_POTS] =
{
    25,
    26,
    27
};

enum class PumpActiveLevel : uint8_t
{
    ACTIVE_LOW = 0,
    ACTIVE_HIGH
};

// Deine D4184-Module sind HIGH-aktiv
constexpr PumpActiveLevel PUMP_ACTIVE_LEVEL =
    PumpActiveLevel::ACTIVE_HIGH;

constexpr uint32_t MAX_PUMP_RUNTIME = 15000UL;

//=========================================================
// Bewässerung
//=========================================================



//=========================================================
// System
//=========================================================

constexpr uint32_t WATCHDOG_TIMEOUT = 30000UL;

#endif
