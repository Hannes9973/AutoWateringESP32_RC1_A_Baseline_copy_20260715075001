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
html += "<html>";
html += "<head>";
html += "<meta charset='UTF-8'>";
html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
html += "<meta http-equiv='refresh' content='2'>";
html += "<title>AutoWatering RC1</title>";

html += "<style>";

html += "body{";
html += "font-family:Arial,Helvetica,sans-serif;";
html += "background:#f0f2f5;";
html += "margin:20px;";
html += "}";

html += "h1{";
html += "text-align:center;";
html += "}";

html += ".card{";
html += "background:white;";
html += "border-radius:12px;";
html += "padding:15px;";
html += "margin-bottom:20px;";
html += "box-shadow:0 2px 8px rgba(0,0,0,0.15);";
html += "}";

html += ".title{";
html += "font-size:22px;";
html += "font-weight:bold;";
html += "margin-bottom:10px;";
html += "}";

html += ".value{";
html += "font-size:18px;";
html += "margin:4px 0;";
html += "}";
html += ".idle{color:green;font-weight:bold;}";
html += ".watering{color:blue;font-weight:bold;}";
html += ".wait{color:orange;font-weight:bold;}";
html += ".error{color:red;font-weight:bold;}";
html += "</style>";

html += "</head>";
html += "<body>";

html += "<h1>🌱 AutoWatering RC1</h1>";

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
            html += "<div class='card'>";

    html += "<div class='title'>Topf ";
    html += String(i + 1);
    html += "<br>";

html += "<a href='/reset?pot=";
html += String(i);
html += "'>";

html += "<button style='padding:8px 20px;'>Reset</button>";
html += "&nbsp;";

html += "<a href='/pumpon?pot=";
html += String(i);
html += "'>";
html += "<button style='padding:8px 20px;'>Pumpe EIN</button>";
html += "</a>";

html += "&nbsp;";

html += "<a href='/pumpoff?pot=";
html += String(i);
html += "'>";
html += "<button style='padding:8px 20px;'>Pumpe AUS</button>";
html += "</a>";

html += "</a>";
    html += "</div>";

    html += "<div class='value'><b>Gewicht:</b> ";
    html += String(Pot[i].getWeight(), 1);
    html += " g</div>";

    html += "<div class='value'><b>Status:</b> ";

switch(Pot[i].getState())
{
    case PotState::IDLE:
        html += "<span class='idle'>IDLE</span>";
        break;

    case PotState::WAIT_FOR_DRY:
        html += "<span class='wait'>WAIT_FOR_DRY</span>";
        break;

    case PotState::WATERING:
        html += "<span class='watering'>WATERING</span>";
        break;

    case PotState::ERROR_SENSOR:
    case PotState::ERROR_TIMEOUT:
  
        html += "<span class='error'>";
        html += Pot[i].getStateName();
        html += "</span>";
        break;

    default:
        html += Pot[i].getStateName();
        break;
}

html += "</div>";

    html += "<form action='/save' method='get'>";

html += "<input type='hidden' name='pot' value='";
html += String(i);
html += "'>";

html += "<div class='value'><b>Startgewicht:</b><br>";
html += "<input type='number' step='0.1' name='start' value='";
html += String(Pot[i].getStartWeight(),1);
html += "'></div>";

html += "<div class='value'><b>Zielgewicht:</b><br>";
html += "<input type='number' step='0.1' name='target' value='";
html += String(Pot[i].getTargetWeight(),1);
html += "'></div>";

html += "<br>";

html += "<input type='submit' value='Speichern'>";

html += "</form>";

    html += "</div>";
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
server.on("/", []()
{
    server.send(200, "text/plain", "Reset-Test OK");
});

server.on("/reset", []()
{
    Serial.println("RESET aufgerufen");

    server.send(200, "text/html",
    "<html><body>"
    "<h2>Reset erfolgreich</h2>"
    "<script>"
    "setTimeout(function(){window.location='/'},300);"
    "</script>"
    "</body></html>");
});



server.on("/pumpon", []()
{
    if(server.hasArg("pot"))
    {
        int p = server.arg("pot").toInt();

        if(p >= 0 && p < NUMBER_OF_POTS)
        {
            Pump.on(p);
        }
    }

    server.sendHeader("Location", "/");
    server.send(303);
});

server.on("/pumpoff", []()
{
    if(server.hasArg("pot"))
    {
        int p = server.arg("pot").toInt();

        if(p >= 0 && p < NUMBER_OF_POTS)
        {
            Pump.off(p);
        }
    }

    server.sendHeader("Location", "/");
    server.send(303);
});
server.on("/save", []()
{
    if(server.hasArg("pot") &&
       server.hasArg("start") &&
       server.hasArg("target"))
    {
        int p = server.arg("pot").toInt();

        if(p >= 0 && p < NUMBER_OF_POTS)
        {
            float start = server.arg("start").toFloat();
            float target = server.arg("target").toFloat();

            Pot[p].setStartWeight(start);
            Pot[p].setTargetWeight(target);

            Storage.savePotConfig(p, Pot[p]);

            Serial.print("Topf ");
            Serial.print(p + 1);
            Serial.println(" gespeichert.");
        }
    }

    server.send(200, "text/html",
        "<html><body>"
        "<h2>Gespeichert</h2>"
        "<script>"
        "setTimeout(function(){window.location='/'},300);"
        "</script>"
        "</body></html>");
});

server.begin();

Serial.println("Webserver gestartet.");

Serial.println("WebManager gestartet.");
Serial.println("System bereit.");
Serial.println("Befehl 'help' fuer Hilfe.");;
}

void loop()
{
    static unsigned long last = 0;

    if(millis() - last > 5000)
    {
        last = millis();
        Serial.println("Loop lebt...");
    }

    server.handleClient();

    Scale.update();
    Pump.update();

    Command.update(Scale, Pump, Storage, Pot);

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        Pot[i].update();
    }

    if(millis() - lastPrint >= 1000)
    {
        lastPrint = millis();
        printStatus();
    }
}