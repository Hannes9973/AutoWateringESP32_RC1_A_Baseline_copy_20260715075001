// Test
/*
=========================================================
 AutoWateringESP32
 CommandManager.cpp
=========================================================
*/

#include "CommandManager.h"
#include "ScaleManager.h"
#include "PumpManager.h"
#include "StorageManager.h"
#include <cstring>

CommandManager::CommandManager() : _index(0)
{
    memset(_buffer,0,sizeof(_buffer));
}

void CommandManager::begin(){}

bool CommandManager::equals(const char* text) const
{
    return strcmp(_buffer,text)==0;
}

void CommandManager::printHelp() const
{
    Serial.println("help");
    Serial.println("status");
    Serial.println("version");
    Serial.println("info");
    Serial.println("tare 1|2|3");
    Serial.println("save");
    Serial.println("load");
    Serial.println("pump 1 on/off");
    Serial.println("pump 2 on/off");
    Serial.println("pump 3 on/off");
    Serial.println("pump all off");
}

void CommandManager::printVersion() const
{
    Serial.println("Version-Funktion folgt.");
}

void CommandManager::printInfo() const
{
    Serial.println("Info-Funktion folgt.");
}

void CommandManager::processCommand(ScaleManager& scale,
                                    PumpManager& pump,
                                    StorageManager& storage)
{
    if(equals("help")) printHelp();
    else if(equals("version")) printVersion();
    else if(equals("info")) printInfo();
    else if(equals("status"))
    {
        for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
        {
            Serial.print("Topf ");
            Serial.print(i+1);
            Serial.print(": ");
            Serial.print(scale.getWeight(i),1);
            Serial.print(" g Pump=");
            Serial.println(pump.isRunning(i)?"EIN":"AUS");
        }
    }
    else if(equals("tare 1")) scale.tare(0);
    else if(equals("tare 2")) scale.tare(1);
    else if(equals("tare 3")) scale.tare(2);
    else if(equals("save"))
        for(uint8_t i=0;i<NUMBER_OF_POTS;i++) storage.saveScale(i,scale);
    else if(equals("load"))
        for(uint8_t i=0;i<NUMBER_OF_POTS;i++) storage.loadScale(i,scale);
    else if(equals("pump 1 on")) pump.on(0);
    else if(equals("pump 1 off")) pump.off(0);
    else if(equals("pump 2 on")) pump.on(1);
    else if(equals("pump 2 off")) pump.off(1);
    else if(equals("pump 3 on")) pump.on(2);
    else if(equals("pump 3 off")) pump.off(2);
    else if(equals("pump all off")) pump.stopAll();
    else Serial.println("Unbekannter Befehl.");
}

void CommandManager::update(ScaleManager& scale,
                            PumpManager& pump,
                            StorageManager& storage)
{
    while(Serial.available())
    {
        char c=Serial.read();
        if(c=='\r'||c=='\n')
        {
            if(_index>0)
            {
                _buffer[_index]='\0';
                processCommand(scale,pump,storage);
                _index=0;
            }
        }
        else if(_index<sizeof(_buffer)-1)
        {
            _buffer[_index++]=c;
        }
    }
}
