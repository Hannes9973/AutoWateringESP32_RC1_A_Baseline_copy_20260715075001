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
bool isVerbose() const;
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
bool _verbose = true;
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
bool startsWith(const char* text) const;
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