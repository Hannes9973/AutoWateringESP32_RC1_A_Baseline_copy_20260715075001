#pragma once

#include <Arduino.h>
#include <WebServer.h>

#include "Config.h"
#include "PotManager.h"
#include "PumpManager.h"
#include "StorageManager.h"

class WebManager
{
public:

    WebManager();

    void begin(PotManager pot[],
               PumpManager& pump,
               StorageManager& storage);

    void update();

private:

    WebServer _server;

    PotManager* _pot;
    PumpManager* _pump;
    StorageManager* _storage;

    String createWebPage();

    void setupRoutes();
};