#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>

#include "PotManager.h"
#include "PumpManager.h"
#include "StorageManager.h"

class WebManager
{
public:
    WebManager();

    void begin(PotManager pot[],
               PumpManager* pump,
               StorageManager* storage);

    void update();

private:
    WebServer _server;

    PotManager* _pot;
    PumpManager* _pump;
    StorageManager* _storage;

    void setupRoutes();
    String createWebPage();
};

#endif