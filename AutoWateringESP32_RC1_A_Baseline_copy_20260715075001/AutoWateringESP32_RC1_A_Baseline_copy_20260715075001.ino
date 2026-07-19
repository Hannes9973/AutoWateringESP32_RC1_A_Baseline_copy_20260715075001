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
#include <WiFi.h>
#include <WebServer.h>
#include "WebManager.h"
#include <LittleFS.h>
#include "HistoryManager.h"
#include "TimeManager.h"
#include "EventManager.h"
#include "WateringLogManager.h"

ScaleManager Scale;
StorageManager Storage;
PumpManager Pump;
CommandManager Command;
PotManager Pot[NUMBER_OF_POTS];
WebManager Web;
HistoryManager History;

unsigned long lastHistorySave = 0;
unsigned long lastPrint = 0;




void printHeader()

{
    Serial.println();
    Serial.println("========================================");
    Serial.println(" AutoWateringESP32 V0.7.1");
    Serial.println("========================================");
}


void printStatus()
{if(!Command.isVerbose())
{
    return;
}
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
    if(WIFI_ENABLED)
{
    Serial.println();
    Serial.println("Verbinde mit WLAN...");

    if(!LittleFS.begin(true))
{
    Serial.println("LittleFS konnte nicht gestartet werden.");
}
else
{
    Serial.println("LittleFS bereit.");
}
    WiFi.mode(WIFI_STA);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

while(WiFi.status() != WL_CONNECTED)
{
    delay(500);
    Serial.print(".");
}
Serial.print("IP-Adresse: ");
Serial.println(WiFi.localIP());
Serial.println();
Serial.println("WLAN verbunden.");

Time.begin();
Serial.print("Unix-Zeit: ");
Serial.println(Time.now());

if(Time.isValid())
{
    Serial.println("Zeit synchronisiert.");
}
else
{
    Serial.println("Keine Zeit verfuegbar.");
}
    printHeader();

    Scale.begin();
    Pump.begin();
    History.begin();
    Events.begin();
    Storage.begin();
    Command.begin();
    

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
{
    Storage.loadScale(i, Scale);

    Pot[i].begin(i, &Scale, &Pump);

    Pot[i].setStartWeight(DEFAULT_START_WEIGHT);
    Pot[i].setTargetWeight(DEFAULT_TARGET_WEIGHT);

    Storage.loadPotConfig(i, Pot[i]);
}

Web.begin(Pot, &Pump, &Storage);
WateringLog.begin();

Serial.println("System bereit.");
Serial.println("Befehl 'help' fuer Hilfe.");
}

 }     








void loop()
{
    Web.update();

    Scale.update();
    Pump.update();

    Command.update(Scale, Pump, Storage, Pot);

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        Pot[i].update();
        Events.update(i, Pot[i].getWeight());
    }

    if(millis() - lastHistorySave >= 60000)
    {
        lastHistorySave = millis();

        for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
        {
            History.append(i, Pot[i].getWeight());
        }
    }

    if(millis() - lastPrint >= 1000)
    {
        lastPrint = millis();
        printStatus();
    }
}