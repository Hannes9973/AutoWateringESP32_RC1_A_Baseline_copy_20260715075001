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
#include "PotManager.h"
CommandManager::CommandManager() : _index(0)
{
    memset(_buffer,0,sizeof(_buffer));
}

void CommandManager::begin(){}

bool CommandManager::equals(const char* text) const
{
    return strcmp(_buffer,text)==0;
}
bool CommandManager::isVerbose() const
{
    return _verbose;
}
bool CommandManager::startsWith(const char* text) const
{
    return strncmp(_buffer, text, strlen(text)) == 0;
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
                                    StorageManager& storage,
                                    PotManager pot[])
{
    if(equals("help")) printHelp();
    else if(equals("version")) printVersion();
    else if(equals("info")) printInfo();
    else if(equals("status"))
{
    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        Serial.print("Topf ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(scale.getWeight(i), 1);
        Serial.print(" g ");

        Serial.print("State=");
        Serial.print(pot[i].getStateName());

        Serial.print(" Waterings=");
        Serial.print(pot[i].getStatus().wateringCount);

        Serial.print(" Errors=");
        Serial.print(pot[i].getErrorCount());

        Serial.print(" Pump=");
        Serial.println(pump.isRunning(i) ? "EIN" : "AUS");
    }
}else if(startsWith("pot "))
{
    int potIndex = atoi(_buffer + 4) - 1;

    if(potIndex < 0 || potIndex >= NUMBER_OF_POTS)
    {
        Serial.println("Ungueltige Topfnummer");
    }
    else
    {
        Serial.print("Topf ");
        Serial.println(potIndex + 1);
         Serial.print("State: ");
    Serial.println(pot[potIndex].getStateName());
    Serial.print("Weight: ");
Serial.print(pot[potIndex].getWeight(), 1);
Serial.println(" g");
Serial.print("Start: ");
Serial.print(pot[potIndex].getStartWeight(),1);
Serial.println(" g");

Serial.print("Target: ");
Serial.print(pot[potIndex].getTargetWeight(), 1);
Serial.println(" g");

Serial.print("Auto: ");
Serial.println(pot[potIndex].getAutoMode() ? "ON" : "OFF");

Serial.print("Pump: ");
Serial.println(pump.isRunning(potIndex) ? "ON" : "OFF");

Serial.print("Waterings: ");
Serial.println(pot[potIndex].getStatus().wateringCount);

Serial.print("Errors: ");
Serial.println(pot[potIndex].getErrorCount());
    }
}

else if(startsWith("reset "))
{
    int potIndex = atoi(_buffer + 6) - 1;

    if(potIndex < 0 || potIndex >= NUMBER_OF_POTS)
    {
        Serial.println("Ungueltige Topfnummer");
    }
    else
    {
        pot[potIndex].resetState();

        Serial.print("Topf ");
        Serial.print(potIndex + 1);
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
                            StorageManager& storage,
                            PotManager pot[])
{
    while(Serial.available())
    {
        char c=Serial.read();
        if(c=='\r'||c=='\n')
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
