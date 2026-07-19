#include "WebManager.h"
#include <LittleFS.h>
#include "HistoryManager.h"
#include "HistoryManager.h"
#include "WateringLogManager.h"

extern HistoryManager History;
extern WateringLogManager WateringLog;

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

    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS konnte nicht gestartet werden!");
        return;
    }

    Serial.println("LittleFS bereit.");

    setupRoutes();
    _server.begin();
    _history.begin();
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
    _server.send(200, "text/html", createWebPage());
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
    _server.on("/history", [this]()
{
    if(!_server.hasArg("pot"))
    {
        _server.send(400, "text/plain", "missing pot");
        return;
    }

    int pot = _server.arg("pot").toInt() - 1;

    if(pot < 0 || pot >= NUMBER_OF_POTS)
    {
        _server.send(400, "text/plain", "invalid pot");
        return;
    }

    std::vector<HistoryPoint> history;

    if(!History.load(pot, history))
    {
        _server.send(200, "application/json", "[]");
        return;
    }

    String json = "[";

    for(size_t i = 0; i < history.size(); i++)
    {
        if(i > 0)
            json += ",";

        json += "{\"t\":";
        json += history[i].timestamp;
        json += ",\"w\":";
        json += String(history[i].weight, 1);
        json += "}";
    }

    json += "]";

    _server.send(200, "application/json", json);
    
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

_server.on("/watering", HTTP_GET, [this]()
{
    if(!_server.hasArg("pot"))
    {
        _server.send(400, "text/plain", "missing pot");
        return;
    }

    int pot = _server.arg("pot").toInt() - 1;

    if(pot < 0 || pot >= NUMBER_OF_POTS)
    {
        _server.send(400, "text/plain", "invalid pot");
        return;
    }

    std::vector<WateringPoint> watering;

    if(!WateringLog.load(pot, watering))
    {
        _server.send(200, "application/json", "[]");
        return;
    }

    String json = "[";

    for(size_t i = 0; i < watering.size(); i++)
    {
        if(i > 0)
            json += ",";

        json += "{\"timestamp\":";
        json += watering[i].timestamp;
        json += ",\"amount\":";
        json += String(watering[i].amount, 1);
        json += "}";
    }

    json += "]";

    _server.send(200, "application/json", json);
});
}
//=========================================================

String WebManager::createWebPage()
{
    String html;

    html.reserve(25000);

    html += "<!DOCTYPE html>";
    html += "<html>";

    //-------------------------------------------------
    // HEAD
    //-------------------------------------------------

    html += "<head>";

    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";

    html += "<title>AutoWatering RC2</title>";

    //-------------------------------------------------
    // Chart.js
    //-------------------------------------------------

    html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";

    //-------------------------------------------------
    // CSS
    //-------------------------------------------------

    html += "<style>";

    html += "body{";
    html += "font-family:Arial;";
    html += "background:#f0f2f5;";
    html += "margin:20px;";
    html += "}";

    html += "h1{";
    html += "text-align:center;";
    html += "}";

    html += "h2{";
    html += "margin-top:30px;";
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

    //-------------------------------------------------
    // BODY
    //-------------------------------------------------

    html += "<body>";

    html += "<h1>🌱 AutoWatering RC2</h1>";
    html += "<div class='card'>";
html += "<h2>Gewichtsverlauf Topf 1</h2>";
html += "<canvas id='historyChart'></canvas>";
html += "</div>";

    //-------------------------------------------------
    // Karten
    //-------------------------------------------------
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
html += "<h2>Gewichtsverlauf</h2>";
   html += "<canvas id='weightChart' width='800' height='300' style='border:1px solid #888;'></canvas>";

html += "<script>";

html += "let history=[[]];";
html += "let labels=[];";
html += "let lastWeight=[null];";
html += "let chart=null;";

html += "fetch('/history?pot=1')";
html += ".then(r=>r.json())";
html += ".then(data=>{";

html += "history[0]=data.map(p=>p.w);";
html += "labels=data.map(p=>new Date(p.t*1000).toLocaleTimeString([], {hour:'2-digit',minute:'2-digit'}));";

html += "const ctx=document.getElementById('historyChart');";

html += "chart=new Chart(ctx,{";
html += "type:'line',";
html += "data:{";
html += "labels:labels,";
html += "datasets:[{";
html += "label:'Gewicht (g)',";
html += "data:history[0],";
html += "borderColor:'blue',";
html += "fill:false";
html += "}]";
html += "},";
html += "options:{";
html += "responsive:true,";
html += "animation:false";
html += "}";
html += "});";

html += "updateStatus();";
html += "setInterval(updateStatus,2000);";

html += "});";

html += "async function updateStatus(){";

html += "const r=await fetch('/api/status');";
html += "const d=await r.json();";

html += "for(let i=0;i<d.pots.length;i++){";

html += "document.getElementById('weight'+i).innerHTML=d.pots[i].weight.toFixed(1);";
html += "document.getElementById('state'+i).innerHTML=d.pots[i].state;";
html += "document.getElementById('state'+i).className=d.pots[i].stateClass;";

html += "if(lastWeight[i]===null || Math.abs(lastWeight[i]-d.pots[i].weight)>=0.1){";

html += "lastWeight[i]=d.pots[i].weight;";

html += "if(i===0 && chart){";

html += "history[0].push(d.pots[i].weight);";
html += "labels.push(new Date().toLocaleTimeString([], {hour:'2-digit',minute:'2-digit'}));";

html += "if(history[0].length>200){history[0].shift();labels.shift();}";

html += "chart.data.labels=labels;";
html += "chart.data.datasets[0].data=history[0];";
html += "chart.update('none');";

html += "}";

html += "}";

html += "}";

html += "}";

html += "</script>";


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