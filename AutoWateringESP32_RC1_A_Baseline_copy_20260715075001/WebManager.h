#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>

#include "PotManager.h"
#include "PumpManager.h"
#include "StorageManager.h"
#include "HistoryManager.h"
#include "FirmwareUpdateManager.h"


class WebManager
{
public:
    WebManager();

    void begin(PotManager pot[],
               PumpManager* pump,
               StorageManager* storage);

    void update();
WebServer& getServer();

private:
    WebServer _server;

    PotManager* _pot;
    PumpManager* _pump;
    StorageManager* _storage;
    HistoryManager _history;
FirmwareUpdateManager _firmwareUpdate;

    void setupRoutes();
    String createWebPage();
    String createStatusJson();
};
#endif