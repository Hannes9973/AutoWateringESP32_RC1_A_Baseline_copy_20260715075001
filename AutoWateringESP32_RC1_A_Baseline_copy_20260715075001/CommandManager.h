#pragma once

#include <Arduino.h>

class ScaleManager;
class PumpManager;
class StorageManager;
class PotManager;

class CommandManager
{
public:
    CommandManager();

    bool isVerbose() const;
    bool isMonitor() const;

    void begin();

    void update(ScaleManager& scale,
                PumpManager& pump,
                StorageManager& storage,
                PotManager pot[]);

private:
    char _buffer[64];
    uint8_t _index;

    bool _verbose = true;
    bool _monitor = true;

    void processCommand(ScaleManager& scale,
                        PumpManager& pump,
                        StorageManager& storage,
                        PotManager pot[]);

    bool equals(const char* text) const;
    bool startsWith(const char* text) const;

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
