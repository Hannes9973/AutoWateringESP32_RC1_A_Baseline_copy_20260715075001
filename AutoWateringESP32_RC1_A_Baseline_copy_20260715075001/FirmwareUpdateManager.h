#ifndef FIRMWARE_UPDATE_MANAGER_H
#define FIRMWARE_UPDATE_MANAGER_H

#include <WebServer.h>

class FirmwareUpdateManager
{
public:
    FirmwareUpdateManager();

    void begin(WebServer& server);

private:
    WebServer* _server;

    void registerRoutes();

    void handleUpload();
    void handleUploadFinished();
};

#endif