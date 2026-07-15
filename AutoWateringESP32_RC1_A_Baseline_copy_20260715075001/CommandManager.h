#pragma once

#include <Arduino.h>

class ScaleManager;
class PumpManager;
class StorageManager;
class PotManager;

//=========================================================
// CommandManager
//=========================================================

class CommandManager
{
public:

    //-----------------------------------------------------
    // Konstruktor
    //-----------------------------------------------------

    CommandManager();

    //-----------------------------------------------------
    // Initialisierung
    //-----------------------------------------------------

    void begin();

    //-----------------------------------------------------
    // zyklischer Aufruf
    //-----------------------------------------------------

    void update(ScaleManager& scale,
                PumpManager& pump,
                StorageManager& storage,
                PotManager pot[]);

private:

    //-----------------------------------------------------
    // Eingabepuffer
    //-----------------------------------------------------

    char _buffer[64];

    uint8_t _index;

    //-----------------------------------------------------
    // Kommandoverarbeitung
    //-----------------------------------------------------

    void processCommand(ScaleManager& scale,
                        PumpManager& pump,
                        StorageManager& storage,
                        PotManager pot[]);

    //-----------------------------------------------------
    // Hilfsfunktionen
    //-----------------------------------------------------

    bool equals(const char* text) const;

    //-----------------------------------------------------
    // Ausgaben
    //-----------------------------------------------------

    void printHelp() const;

    void printVersion() const;

    void printInfo() const;

    void printStatus(ScaleManager& scale,
                     PumpManager& pump,
                     PotManager pot[]);

    void printPot(uint8_t potNumber,
                  ScaleManager& scale,
                  PumpManager& pump,
                  PotManager pot[]);
};