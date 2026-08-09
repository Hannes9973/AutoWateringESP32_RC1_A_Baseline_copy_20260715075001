#include "EventLogger.h"
#include "TimeManager.h"

EventLogger Logger;

//------------------------------------------------------
// Initialisierung
//------------------------------------------------------

void EventLogger::begin()
{
    add("EventLogger gestartet");
}
//------------------------------------------------------
// Zeitstempel erzeugen
//------------------------------------------------------

String EventLogger::createTimestamp() const
{
    if(Time.isValid())
    {
        return Time.getDateTimeString();
    }

    return "[--:--:--]";
}
//------------------------------------------------------
// Neues Ereignis hinzufügen
//------------------------------------------------------



void EventLogger::add(const String& text)
{
    String entry;

    if(Time.isValid())
    {
        entry =
            Time.getDateTimeString()
            + "  "
            + text;
    }
    else
    {
        entry =
            "[Zeit unbekannt] "
            + text;
    }

    if(_count < EVENT_LOG_SIZE)
    {
        _events[_count] = entry;
        _count++;
    }
    else
    {
        for(uint8_t i=1;i<EVENT_LOG_SIZE;i++)
        {
            _events[i-1]=_events[i];
        }

        _events[EVENT_LOG_SIZE-1]=entry;
    }

    Serial.print("[EVENT] ");
    Serial.println(entry);
}

    

//------------------------------------------------------
// Ereignis lesen
//------------------------------------------------------

String EventLogger::get(uint8_t index) const
{
    if(index >= _count)
        return "";

    return _events[index];
}

//------------------------------------------------------
// Anzahl Einträge
//------------------------------------------------------

uint8_t EventLogger::count() const
{
    return _count;
}