#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "FRITZ!Box 7491";
const char* password = "20946658577422564934";

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    Serial.print("Verbinde");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WLAN verbunden");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA.setHostname("OTA_TEST");

    // KEIN Passwort!
    // ArduinoOTA.setPassword("123456");

    ArduinoOTA.onStart([]() {
        Serial.println("OTA Start");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("OTA Ende");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("%u %%\r", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Fehler %u\n", error);
    });
Serial.println("Vor begin");
    ArduinoOTA.begin();
Serial.println("Nach begin");
    Serial.println("OTA bereit");
    Serial.print("MAC: ");
Serial.println(WiFi.macAddress());

Serial.print("Hostname: ");
Serial.println(WiFi.getHostname());
}

void loop()
{
    ArduinoOTA.handle();
}