/*
=========================================================
 AutoWateringESP32
 Version 0.7.1
=========================================================
*/

#include <Arduino.h>

#include "Config.h"
#include "Types.h"
#include "ScaleManager.h"
#include "StorageManager.h"
#include "PumpManager.h"
#include "CommandManager.h"
#include "PotManager.h"

ScaleManager Scale;
StorageManager Storage;
PumpManager Pump;
CommandManager Command;
PotManager Pot[NUMBER_OF_POTS];

unsigned long lastPrint = 0;

void printHeader()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println(" AutoWateringESP32 V0.7.1");
    Serial.println("========================================");
}

void printStatus()
{
    for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
    {
        Serial.print("Topf ");
        Serial.print(i+1);
        Serial.print(": ");

        if(!Scale.isReady(i))
        {
            Serial.println("HX711 nicht bereit");
            continue;
        }

        Serial.print(Scale.getWeight(i),1);
        Serial.print(" g RAW=");
        Serial.print(Scale.getRaw(i));
        Serial.print(" Pump=");
        Serial.println(Pump.isRunning(i) ? "EIN" : "AUS");
    }

    Serial.println("----------------------------------------");
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    delay(1000);

    printHeader();

    Scale.begin();
    Pump.begin();
    Storage.begin();
    Command.begin();

    for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
    {
        Storage.loadScale(i, Scale);

        Pot[i].begin(i, &Scale, &Pump);
        Pot[i].setStartWeight(DEFAULT_START_WEIGHT);
        Pot[i].setTargetWeight(DEFAULT_TARGET_WEIGHT);
    }

    Serial.println("System bereit.");
    Serial.println("Befehl 'help' fuer Hilfe.");
}

void loop()
{
    Scale.update();
    Pump.update();

    Command.update(Scale, Pump, Storage, Pot);

    for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
    {
        Pot[i].update();
    }

    if(millis()-lastPrint>=1000)
    {
        lastPrint=millis();
        printStatus();
    }
}
