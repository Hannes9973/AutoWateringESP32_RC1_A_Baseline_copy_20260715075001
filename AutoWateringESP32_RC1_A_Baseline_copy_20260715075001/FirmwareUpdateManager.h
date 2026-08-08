#ifndef FIRMWARE_UPDATE_MANAGER_H
#define FIRMWARE_UPDATE_MANAGER_H

#include <WebServer.h>

class FirmwareUpdateManager
{
public:
    void begin(WebServer& server);

private:
    static void handleUpload(WebServer& server);
};

#endif