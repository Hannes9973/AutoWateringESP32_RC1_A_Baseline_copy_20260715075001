#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <Arduino.h>

#define EVENT_LOG_SIZE 50

class EventLogger
{
public:

    void begin();

    void add(const String& text);

    String get(uint8_t index) const;

    uint8_t count() const;
    String createTimestamp() const;

private:

    String _events[EVENT_LOG_SIZE];

    uint8_t _count = 0;

};

extern EventLogger Logger;
#endif