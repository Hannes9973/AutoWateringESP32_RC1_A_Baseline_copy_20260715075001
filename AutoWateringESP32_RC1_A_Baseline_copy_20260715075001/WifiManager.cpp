#include "WiFiManager.h"

#include <WiFi.h>

#include "Config.h"
#include "EventLogger.h"

WiFiManager WiFiMonitor;

static volatile uint8_t _lastDisconnectReason = 0;
static volatile bool _wifiDisconnectEvent = false;

static int _lastRSSI = 0;
static int _lastChannel = 0;
static String _lastBSSID = "";

// ============================================================
// WLAN Event
// ============================================================

static void WiFiEventHandler(WiFiEvent_t event, WiFiEventInfo_t info)
{
    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED)
    {
        _lastDisconnectReason =
            info.wifi_sta_disconnected.reason;

        _wifiDisconnectEvent = true;
    }
}


// ============================================================
// BEGIN
// ============================================================

void WiFiManager::begin()
{
    WiFi.onEvent(
        WiFiEventHandler,
        ARDUINO_EVENT_WIFI_STA_DISCONNECTED
    );

    WiFi.mode(WIFI_STA);

    // WLAN-Energiesparen deaktiviert
    WiFi.setSleep(false);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    _wasConnected = false;
    _lastCheck = millis();

    Serial.println("WiFiManager gestartet");
}


// ============================================================
// UPDATE
// ============================================================

void WiFiManager::update()
{
    // --------------------------------------------------------
    // WLAN-Disconnect-Event auswerten
    // --------------------------------------------------------

    if (_wifiDisconnectEvent)
    {
        uint8_t reason = _lastDisconnectReason;

        _wifiDisconnectEvent = false;

        Serial.println();
        Serial.println("========== WLAN DIAGNOSE ==========");
        Serial.println("WLAN verloren");

        Serial.print("WiFi Status: ");
        Serial.println(WiFi.status());

        Serial.print("Disconnect Reason: ");
        Serial.println(reason);

        Serial.print("Letzter RSSI: ");
        Serial.println(_lastRSSI);

        Serial.print("Letzter Kanal: ");
        Serial.println(_lastChannel);

        Serial.print("Letzte BSSID: ");
        Serial.println(_lastBSSID);

        Serial.print("Aktueller RSSI: ");
        Serial.println(WiFi.RSSI());

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        Serial.println("===================================");
    }


    // --------------------------------------------------------
    // Nur alle 10 Sekunden prüfen
    // --------------------------------------------------------

    if (millis() - _lastCheck < 10000)
        return;

    _lastCheck = millis();


    // --------------------------------------------------------
    // Aktuellen WLAN-Status ermitteln
    // --------------------------------------------------------

    bool connected =
        (WiFi.status() == WL_CONNECTED);


    // --------------------------------------------------------
    // WLAN-Diagnosedaten aktualisieren
    // --------------------------------------------------------

    if (connected)
    {
        _lastRSSI = WiFi.RSSI();
        _lastChannel = WiFi.channel();
        _lastBSSID = WiFi.BSSIDstr();
    }


    // --------------------------------------------------------
    // WLAN verloren
    // --------------------------------------------------------

    if (!connected && _wasConnected)
    {
        Logger.add("WLAN verloren");

        Serial.println("WiFiManager: Reconnect gestartet");

        // Kein WiFi.disconnect() mehr!
        // Dadurch vermeiden wir den vorher beobachteten
        // zusätzlichen Disconnect-/Reason-8-Effekt.

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }


    // --------------------------------------------------------
    // WLAN wieder verbunden
    // --------------------------------------------------------

    if (connected && !_wasConnected)
    {
        Logger.add("WLAN wieder verbunden");

        Serial.println();
        Serial.println("WiFiManager: WLAN wieder verbunden");

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        Serial.print("RSSI: ");
        Serial.println(WiFi.RSSI());
    }


    // --------------------------------------------------------
    // Zustand merken
    // --------------------------------------------------------

    _wasConnected = connected;
}