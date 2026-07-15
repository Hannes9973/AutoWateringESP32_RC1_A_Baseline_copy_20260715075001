#pragma once

/*
=========================================================
 AutoWateringESP32
 Types.h

 Gemeinsame Datentypen
=========================================================
*/

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>
#include "Config.h"
#include "VERSION.h"
//=========================================================
// Firmware Version
//=========================================================

struct Version
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
};

//=========================================================
// HX711
//=========================================================

enum class ScaleState : uint8_t
{
    NOT_READY = 0,
    READY,
    NO_LOADCELL,
    ERROR
};

//=========================================================
// Pumpe
//=========================================================

enum class PumpState : uint8_t
{
    OFF = 0,
    ON,
    TIMEOUT,
    ERROR
};

//=========================================================
// Topf
//=========================================================

enum class PotState : uint8_t
{
    IDLE = 0,

    WAIT_FOR_DRY,

    WATERING,

    TARGET_REACHED,

    MANUAL,

    ERROR_SENSOR,

    ERROR_TIMEOUT,

    ERROR_TANK
};

//=========================================================
// Scale Status
//=========================================================

struct ScaleStatus
{
    ScaleState state = ScaleState::NOT_READY;

    long raw = 0;

    long offset = 0;

    float calibration = DEFAULT_CALIBRATION_FACTOR;

    float weight = 0.0f;

    bool ready = false;
};

//=========================================================
// Pump Status
//=========================================================

struct PumpStatus
{
    PumpState state = PumpState::OFF;

    bool running = false;

    uint32_t startTime = 0;

    uint32_t runTime = 0;
};

//=========================================================
// Pot Status
//=========================================================

struct PotStatus
{
    PotState state = PotState::IDLE;

    bool autoMode = true;

    bool watering = false;

    float weight = 0.0f;

    float startWeight = DEFAULT_START_WEIGHT;

    float targetWeight = DEFAULT_TARGET_WEIGHT;

    uint32_t wateringTime = 0;

float wateringStartWeight = 0.0f;

uint32_t wateringCount = 0;
uint32_t errorCount = 0;
};

//=========================================================
// Pot Daten
//=========================================================

struct PotData
{
    ScaleStatus scale;

    PumpStatus pump;

    PotStatus status;
};

//=========================================================
// System
//=========================================================

struct SystemStatus
{
    Version version =
    {
        FW_VERSION_MAJOR,
        FW_VERSION_MINOR,
        FW_VERSION_PATCH
    };

    uint32_t uptime = 0;

    uint32_t freeHeap = 0;

    PotData pot[NUMBER_OF_POTS];
};

#endif