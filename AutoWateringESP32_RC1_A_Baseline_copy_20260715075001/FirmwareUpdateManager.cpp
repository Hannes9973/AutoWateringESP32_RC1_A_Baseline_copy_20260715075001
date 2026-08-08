#include "FirmwareUpdateManager.h"

#include <Update.h>
#include <LittleFS.h>

void FirmwareUpdateManager::begin(WebServer& server)
{
    Serial.println("FirmwareUpdateManager gestartet");
    
    server.serveStatic(
        "/update",
        LittleFS,
        "/update.html");

    server.serveStatic(
        "/update.css",
        LittleFS,
        "/update.css");

    server.serveStatic(
        "/update.js",
        LittleFS,
        "/update.js");

    // Upload kommt im nächsten Schritt
}