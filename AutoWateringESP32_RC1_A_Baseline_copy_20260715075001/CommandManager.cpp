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
#include "PotManager.h"

#include <Arduino.h>
#include <cstring>

//=========================================================
// Konstruktor
//=========================================================

CommandManager::CommandManager() :
    _index(0)
{
    memset(_buffer,0,sizeof(_buffer));
}

//=========================================================

void CommandManager::begin()
{
}

//=========================================================

bool CommandManager::equals(const char* text) const
{
    return strcmp(_buffer,text)==0;
}

//=========================================================

bool CommandManager::startsWith(const char* text) const
{
    return strncmp(_buffer,text,strlen(text))==0;
}

//=========================================================

bool CommandManager::isVerbose() const
{
    return _verbose;
}

//=========================================================
// Hilfe
//=========================================================

void CommandManager::printHelp() const
{
    Serial.println();
    Serial.println("===== Commands =====");

    Serial.println("help");
    Serial.println("version");
    Serial.println("info");
    Serial.println("status");

    Serial.println();

    Serial.println("pot 1");
    Serial.println("pot 2");
    Serial.println("pot 3");

    Serial.println();

    Serial.println("reset 1");
    Serial.println("reset 2");
    Serial.println("reset 3");
    Serial.println("reset all");

    Serial.println();

    Serial.println("verbose on");
    Serial.println("verbose off");

    Serial.println();

    Serial.println("tare 1");
    Serial.println("tare 2");
    Serial.println("tare 3");

    Serial.println();

    Serial.println("save");
    Serial.println("load");

    Serial.println();

    Serial.println("pump 1 on");
    Serial.println("pump 1 off");
    Serial.println("pump 2 on");
    Serial.println("pump 2 off");
    Serial.println("pump 3 on");
    Serial.println("pump 3 off");
    Serial.println("pump all off");

    Serial.println();

    Serial.println("setstart <pot> <g>");
    Serial.println("settarget <pot> <g>");

    Serial.println("====================");
}

//=========================================================

void CommandManager::printVersion() const
{
    Serial.println("AutoWateringESP32 RC1");
}

//=========================================================

void CommandManager::printInfo() const
{
    Serial.println("ESP32 Automatic Watering");
}

//=========================================================
// Status aller Toepfe
//=========================================================

void CommandManager::printStatus(ScaleManager& scale,
                                 PumpManager& pump,
                                 PotManager pot[])
{
    for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
    {
        Serial.print("Topf ");
        Serial.print(i+1);

        Serial.print(": ");

        Serial.print(scale.getWeight(i),1);
        Serial.print(" g ");

        Serial.print("State=");
        Serial.print(pot[i].getStateName());

        Serial.print(" Waterings=");
        Serial.print(pot[i].getStatus().wateringCount);

        Serial.print(" Errors=");
        Serial.print(pot[i].getErrorCount());

        Serial.print(" Pump=");
        Serial.println(pump.isRunning(i) ? "EIN":"AUS");
    }
}

//=========================================================
// Detailausgabe eines Topfes
//=========================================================

void CommandManager::printPot(uint8_t potNumber,
                              ScaleManager& scale,
                              PumpManager& pump,
                              PotManager pot[])
{
    if(potNumber>=NUMBER_OF_POTS)
    {
        Serial.println("Ungueltige Topfnummer");
        return;
    }

    Serial.print("Topf ");
    Serial.println(potNumber+1);

    Serial.print("State: ");
    Serial.println(pot[potNumber].getStateName());

    Serial.print("Weight: ");
    Serial.print(scale.getWeight(potNumber),1);
    Serial.println(" g");

    Serial.print("Start: ");
    Serial.print(pot[potNumber].getStartWeight(),1);
    Serial.println(" g");

    Serial.print("Target: ");
    Serial.print(pot[potNumber].getTargetWeight(),1);
    Serial.println(" g");

    Serial.print("Auto: ");
    Serial.println(pot[potNumber].getAutoMode() ? "ON":"OFF");

    Serial.print("Pump: ");
    Serial.println(pump.isRunning(potNumber) ? "ON":"OFF");

    Serial.print("Waterings: ");
    Serial.println(pot[potNumber].getStatus().wateringCount);

    Serial.print("Errors: ");
    Serial.println(pot[potNumber].getErrorCount());
}
//=========================================================
// Kommandoverarbeitung
//=========================================================

void CommandManager::processCommand(ScaleManager& scale,
                                    PumpManager& pump,
                                    StorageManager& storage,
                                    PotManager pot[])
{
    
    if(equals("help"))
    {
        printHelp();
    }
    else if(equals("version"))
    {
        printVersion();
    }
    else if(equals("info"))
    {
        printInfo();
    }
    else if(equals("status"))
    {
        printStatus(scale,pump,pot);
    }
    else if(startsWith("pot "))
    {
        int potIndex = atoi(_buffer + 4) - 1;
        printPot(potIndex,scale,pump,pot);
    }
    else if(equals("reset all"))
    {
        for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
        {
            pot[i].resetState();
        }

        Serial.println("Alle Toepfe wurden zurueckgesetzt.");
    }
    else if(startsWith("reset "))
    {
        int potIndex = atoi(_buffer + 6) - 1;

        if(potIndex<0 || potIndex>=NUMBER_OF_POTS)
        {
            Serial.println("Ungueltige Topfnummer");
        }
        else
        {
            pot[potIndex].resetState();

            Serial.print("Topf ");
            Serial.print(potIndex+1);
            Serial.println(" wurde zurueckgesetzt.");
        }
    }
    else if(equals("verbose on"))
{
    _verbose = true;
    Serial.println("Verbose ON");
}
else if(equals("verbose off"))
{
    _verbose = false;
    Serial.println("Verbose OFF");
}
else if(startsWith("setstart "))
{
    int potIndex;
    float value;

    if(sscanf(_buffer, "setstart %d %f", &potIndex, &value) == 2)
    {
        potIndex--;

        if(potIndex >= 0 && potIndex < NUMBER_OF_POTS)
        {
            pot[potIndex].setStartWeight(value);

            Serial.print("Topf ");
            Serial.print(potIndex + 1);
            Serial.print(" Startgewicht = ");
            Serial.print(value, 1);
            Serial.println(" g");
        }
        else
        {
            Serial.println("Ungueltige Topfnummer");
        }
    }
    else
    {
        Serial.println("Syntax: setstart <Topf> <Gramm>");
    }
}
else if(equals("tare 1"))
    {
        scale.tare(0);
    }
    else if(equals("tare 2"))
    {
        scale.tare(1);
    }
    else if(equals("tare 3"))
    {
        scale.tare(2);
    }
    else if(equals("save"))
    {
        for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
            storage.saveScale(i,scale);
    }
    else if(equals("load"))
    {
        for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
            storage.loadScale(i,scale);
    }
    else if(equals("pump 1 on"))
    {
        pump.on(0);
    }
    else if(equals("pump 1 off"))
    {
        pump.off(0);
    }
    else if(equals("pump 2 on"))
    {
        pump.on(1);
    }
    else if(equals("pump 2 off"))
    {
        pump.off(1);
    }
    else if(equals("pump 3 on"))
    {
        pump.on(2);
    }
    else if(equals("pump 3 off"))
    {
        pump.off(2);
    }
    else if(equals("pump all off"))
    {
        pump.stopAll();
    }
    else
    {
        Serial.println("Unbekannter Befehl.");
    }
}

//=========================================================
// Serielle Eingabe
//=========================================================

void CommandManager::update(ScaleManager& scale,
                            PumpManager& pump,
                            StorageManager& storage,
                            PotManager pot[])
{
    while(Serial.available())
    {
        char c = Serial.read();

        if(c=='\r' || c=='\n')
        {
            if(_index>0)
            {
                _buffer[_index]='\0';

                processCommand(scale,
                               pump,
                               storage,
                               pot);

                _index=0;
            }
        }
        else if(_index<sizeof(_buffer)-1)
        {
            _buffer[_index++]=c;
        }
    }
}