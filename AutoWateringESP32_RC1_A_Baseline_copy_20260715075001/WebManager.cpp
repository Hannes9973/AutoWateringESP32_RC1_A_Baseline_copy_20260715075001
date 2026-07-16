#include "WebManager.h"

WebManager::WebManager()
    : _server(80),
      _pot(nullptr),
      _pump(nullptr),
      _storage(nullptr)
{
}

void WebManager::begin(PotManager pot[],
                       PumpManager& pump,
                       StorageManager& storage)
{
    _pot = pot;
    _pump = &pump;
    _storage = &storage;
}

void WebManager::update()
{
    _server.handleClient();
}

void WebManager::setupRoutes()
{
}

String WebManager::createWebPage()
{
    return "<html><body>WebManager</body></html>";
}