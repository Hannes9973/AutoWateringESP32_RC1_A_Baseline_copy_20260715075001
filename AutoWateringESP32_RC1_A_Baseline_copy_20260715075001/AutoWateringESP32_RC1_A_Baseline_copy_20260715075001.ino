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

ScaleManager Scale;
StorageManager Storage;
PumpManager Pump;
CommandManager Command;
PotManager Pot[NUMBER_OF_POTS];
WebServer server(80);

unsigned long lastPrint = 0;
String createWebPage()
{
    String html;

    html += "<!DOCTYPE html>";
    html += "<html><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta http-equiv='refresh' content='2'>";
    html += "<title>AutoWatering RC1</title>";
    html += "</head><body>";

    html += "<h1>AutoWatering RC1</h1>";

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        html += "<h2>Topf ";
        html += String(i + 1);
        html += "</h2>";

        html += "Gewicht: ";
        html += String(Pot[i].getWeight(), 1);
        html += " g<br>";

        html += "Status: ";
        html += Pot[i].getStateName();
        html += "<br><br>";
    }

    html += "</body></html>";

    return html;
}



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

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WLAN verbunden");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
}
    delay(1000);

    printHeader();

    Scale.begin();
    Pump.begin();
    Storage.begin();
    Command.begin();

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
{
    Storage.loadScale(i, Scale);

    Pot[i].begin(i, &Scale, &Pump);

    // Nur Standardwerte setzen, falls noch nichts gespeichert wurde
    Pot[i].setStartWeight(DEFAULT_START_WEIGHT);
    Pot[i].setTargetWeight(DEFAULT_TARGET_WEIGHT);

    // Gespeicherte Werte laden (überschreibt ggf. die Standardwerte)
    Storage.loadPotConfig(i, Pot[i]);
}
server.on("/", []()
{
    server.send(200, "text/html", createWebPage());
});

server.begin();

Serial.println("Webserver gestartet.");
Serial.println("System bereit.");
Serial.println("Befehl 'help' fuer Hilfe.");;
}

void loop()
{server.handleClient();
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
