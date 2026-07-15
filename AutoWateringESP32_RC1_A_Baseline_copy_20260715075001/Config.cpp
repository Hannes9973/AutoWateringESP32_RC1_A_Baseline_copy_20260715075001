#include "Config.h"
/*
=========================================================
 AutoWateringESP32
 Config.cpp
=========================================================
*/

#include "Config.h"
#include "VERSION.h"

//=========================================================
// Firmwareinformationen
//=========================================================

void printFirmwareInfo()
{
    Serial.println();
    Serial.println("=================================================");
    Serial.println(FW_NAME);

    Serial.print("Version : ");
    Serial.println(FW_VERSION_STRING);

    Serial.print("Build   : ");
    Serial.println(FW_BUILD);

    Serial.print("Datum   : ");
    Serial.print(FW_BUILD_DATE);
    Serial.print(" ");
    Serial.println(FW_BUILD_TIME);

    Serial.print("Board   : ");
    Serial.println(FW_TARGET);

    Serial.println("=================================================");
    Serial.println();
}
//=========================================================
// Funktionen
//=========================================================

void printFirmwareInfo();