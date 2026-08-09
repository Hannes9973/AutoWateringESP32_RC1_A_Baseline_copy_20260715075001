#pragma once

#include <Arduino.h>

class ResetManager
{
public:
    void begin();

    String getReason() const;

private:
    String _reason;
};

extern ResetManager Reset;