#include "HistoryManager.h"

void HistoryManager::begin()
{
    // momentan nichts notwendig
}

String HistoryManager::fileName(uint8_t pot)
{
    return "/history" + String(pot) + ".csv";
}

void HistoryManager::clear(uint8_t pot)
{
    LittleFS.remove(fileName(pot));
}

bool HistoryManager::append(uint8_t pot, float weight)
{
    std::vector<HistoryPoint> history;

    load(pot, history);

    if(history.size() >= MAX_HISTORY_POINTS)
    {
        history.erase(history.begin());
    }

    HistoryPoint p;
    p.timestamp = Time.now();
    p.weight = weight;

    history.push_back(p);

    File file = LittleFS.open(fileName(pot), FILE_WRITE);

    if(!file)
        return false;

    for(const auto& h : history)
    {
        file.print(h.timestamp);
        file.print(';');
        file.println(h.weight, 1);
    }

    file.close();

    return true;
}

bool HistoryManager::load(uint8_t pot,
                          std::vector<HistoryPoint>& history)
{
    history.clear();

    File file = LittleFS.open(fileName(pot), FILE_READ);

    if (!file)
        return false;

    while (file.available())
    {
        String line = file.readStringUntil('\n');

        line.trim();

        if (line.length() == 0)
            continue;

        int pos = line.indexOf(';');

        if (pos < 0)
            continue;

        HistoryPoint p;

        p.timestamp = line.substring(0, pos).toInt();
        p.weight = line.substring(pos + 1).toFloat();

        history.push_back(p);
    }

    file.close();

    return true;
}