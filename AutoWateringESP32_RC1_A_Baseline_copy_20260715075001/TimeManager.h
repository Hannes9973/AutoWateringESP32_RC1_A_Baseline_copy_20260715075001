#pragma once

#include <Arduino.h>

class TimeManager
{
public:

    void begin();

    uint32_t now();

    bool isValid();

private:

    bool _valid = false;
};

extern TimeManager Time;