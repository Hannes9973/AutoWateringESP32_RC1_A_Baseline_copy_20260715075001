#include "WebManager.h"
#include <LittleFS.h>

WebManager::WebManager()
    : _server(80),
      _pot(nullptr),
      _pump(nullptr),
      _storage(nullptr)
{
}

void WebManager::begin(PotManager pot[],
                       PumpManager* pump,
                       StorageManager* storage)
{
    _pot = pot;
    _pump = pump;
    _storage = storage;
if(!LittleFS.begin(true))
{
    Serial.println("LittleFS konnte nicht gestartet werden!");
}
else
{
    Serial.println("LittleFS bereit.");
}
    setupRoutes();
    _server.begin();
}

void WebManager::update()
{
    _server.handleClient();
}

//=========================================================

void WebManager::setupRoutes()
{
    //-----------------------------------------------------
    // Hauptseite
    //-----------------------------------------------------

   _server.on("/", [this]()
{
    File file = LittleFS.open("/index.html", "r");

    if(!file)
    {
        _server.send(404, "text/plain", "index.html nicht gefunden");
        return;
    }

    _server.streamFile(file, "text/html");
    file.close();
});

    //-----------------------------------------------------
    // Reset
    //-----------------------------------------------------

    _server.on("/reset", [this]()
    {
        if(_server.hasArg("pot"))
        {
            int p = _server.arg("pot").toInt();

            if(p >= 0 && p < NUMBER_OF_POTS)
            {
                _pot[p].resetState();
            }
        }

        _server.send(200,"text/html",
            "<html><body>"
            "<h2>Reset erfolgreich</h2>"
            "<script>"
            "setTimeout(function(){window.location='/'},300);"
            "</script>"
            "</body></html>");
    });
//-----------------------------------------------------
// JSON Status
//-----------------------------------------------------

_server.on("/api/status", [this]()
{
    _server.send(200,
                 "application/json",
                 createStatusJson());
});
    //-----------------------------------------------------
    // Pumpe EIN
    //-----------------------------------------------------

    _server.on("/pumpon",[this]()
    {
        if(_server.hasArg("pot"))
        {
            int p=_server.arg("pot").toInt();

            if(p>=0 && p<NUMBER_OF_POTS)
            {
                _pump->on(p);
            }
        }

        _server.send(200,"text/html",
            "<script>"
            "window.location='/'"
            "</script>");
    });

    //-----------------------------------------------------
    // Pumpe AUS
    //-----------------------------------------------------

    _server.on("/pumpoff",[this]()
    {
        if(_server.hasArg("pot"))
        {
            int p=_server.arg("pot").toInt();

            if(p>=0 && p<NUMBER_OF_POTS)
            {
                _pump->off(p);
            }
        }

        _server.send(200,"text/html",
            "<script>"
            "window.location='/'"
            "</script>");
    });

    //-----------------------------------------------------
    // Speichern
    //-----------------------------------------------------

    _server.on("/save",[this]()
    {
        if(_server.hasArg("pot") &&
           _server.hasArg("start") &&
           _server.hasArg("target"))
        {
            int p=_server.arg("pot").toInt();

            if(p>=0 && p<NUMBER_OF_POTS)
            {
                float start=_server.arg("start").toFloat();
                float target=_server.arg("target").toFloat();

                _pot[p].setStartWeight(start);
                _pot[p].setTargetWeight(target);

                _storage->savePotConfig(p,_pot[p]);

                Serial.print("Topf ");
                Serial.print(p+1);
                Serial.println(" gespeichert.");
            }
        }

        _server.send(200,"text/html",
            "<html><body>"
            "<h2>Gespeichert</h2>"
            "<script>"
            "setTimeout(function(){window.location='/'},300);"
            "</script>"
            "</body></html>");
    });
}

//=========================================================

String WebManager::createWebPage()

{
    String html;

    html.reserve(8000);

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";

    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";


    html += "<title>AutoWatering RC1</title>";

    html += "<style>";

    html += "body{";
    html += "font-family:Arial;";
    html += "background:#f0f2f5;";
    html += "margin:20px;";
    html += "}";

    html += "h1{";
    html += "text-align:center;";
    html += "}";

    html += ".card{";
    html += "background:white;";
    html += "padding:15px;";
    html += "margin-bottom:20px;";
    html += "border-radius:12px;";
    html += "box-shadow:0 2px 8px rgba(0,0,0,.15);";
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
    html += "<script>";

html += "async function updateStatus(){";

html += "const r=await fetch('/api/status');";
html += "const d=await r.json();";

html += "for(let i=0;i<d.pots.length;i++){";

html += "document.getElementById('weight'+i).innerHTML=";
html += "d.pots[i].weight.toFixed(1);";

html += "document.getElementById('state'+i).innerHTML=";
html += "d.pots[i].state;";

html += "}";

html += "}";

html += "}";

html += "setInterval(updateStatus,2000);";
html += "updateStatus();";

html += "</script>";
    html += "<body>";

    html += "<h1>🌱 AutoWatering RC1</h1>";

    //=========================================================

//=========================================================


        //-----------------------------------------------------
    // Karten
    //-----------------------------------------------------

    for(uint8_t i=0;i<NUMBER_OF_POTS;i++)
    {
        html += "<div class='card'>";

        html += "<div class='title'>Topf ";
        html += String(i+1);
        html += "</div>";

        //-------------------------------------------------
        // Gewicht
        //-------------------------------------------------

        html += "<div class='value'><b>Gewicht:</b> <span id='weight";
html += String(i);
html += "'>";
html += String(_pot[i].getWeight(),1);
html += "</span> g</div>";

        //-------------------------------------------------
        // Status
        //-------------------------------------------------

        html += "<div class='value'><b>Status:</b> ";
html += "<span id='state";
html += String(i);
html += "' class='";

switch(_pot[i].getState())
{
    case PotState::IDLE:
        html += "idle";
        break;

    case PotState::WAIT_FOR_DRY:
        html += "wait";
        break;

    case PotState::WATERING:
        html += "watering";
        break;

    case PotState::ERROR_SENSOR:
    case PotState::ERROR_TIMEOUT:
        html += "error";
        break;
}

html += "'>";

html += _pot[i].getStateName();

html += "</span>";

        html += "</div>";

        //-------------------------------------------------
        // Formular
        //-------------------------------------------------

        html += "<form action='/save' method='get'>";

        html += "<input type='hidden' name='pot' value='";
        html += String(i);
        html += "'>";

        html += "<div class='value'><b>Startgewicht:</b><br>";
        html += "<input type='number' step='0.1' name='start' value='";
        html += String(_pot[i].getStartWeight(),1);
        html += "'>";
        html += "</div>";

        html += "<div class='value'><b>Zielgewicht:</b><br>";
        html += "<input type='number' step='0.1' name='target' value='";
        html += String(_pot[i].getTargetWeight(),1);
        html += "'>";
        html += "</div>";

        html += "<br>";

        html += "<input type='submit' value='Speichern'>";

        html += "</form>";

        //-------------------------------------------------
        // Buttons
        //-------------------------------------------------

        html += "<br>";

        html += "<a href='/reset?pot=";
        html += String(i);
        html += "'>";
        html += "<button style='padding:8px 20px;'>Reset</button>";
        html += "</a>";

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

        html += "</div>";
    }

    //-----------------------------------------------------

    html += "</body>";
    html += "</html>";

    return html;
}
String WebManager::createStatusJson()
{
    String json = "{";
    json += "\"pots\":[";

    for(uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        if(i > 0)
            json += ",";

        json += "{";
        json += "\"weight\":" + String(_pot[i].getWeight(), 1) + ",";
        json += "\"start\":" + String(_pot[i].getStartWeight(), 1) + ",";
        json += "\"target\":" + String(_pot[i].getTargetWeight(), 1) + ",";
        json += "\"state\":\"";
json += _pot[i].getStateName();
json += "\",";

json += "\"stateClass\":\"";

switch(_pot[i].getState())
{
    case PotState::IDLE:
        json += "idle";
        break;

    case PotState::WAIT_FOR_DRY:
        json += "wait";
        break;

    case PotState::WATERING:
        json += "watering";
        break;

    case PotState::ERROR_SENSOR:
    case PotState::ERROR_TIMEOUT:
        json += "error";
        break;

    default:
        json += "";
        break;
}

json += "\"}";
    }

    json += "]";
    json += "}";

    return json;
}