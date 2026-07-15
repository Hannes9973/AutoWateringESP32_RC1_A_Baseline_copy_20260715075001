#include "ScaleManager.h"

//=========================================================
// Konstruktor
//=========================================================

ScaleManager::ScaleManager()
{
    for (uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        _raw[i] = 0;
        _offset[i] = 0;

        _weight[i] = 0.0f;

        _calibration[i] = DEFAULT_CALIBRATION_FACTOR;

        _ready[i] = false;
    }
}

//=========================================================
// Initialisierung
//=========================================================

void ScaleManager::begin()
{
    for (uint8_t i = 0; i < NUMBER_OF_POTS; i++)
    {
        _scale[i].begin(
            HX711_DT[i],
            HX711_SCK[i]);

        _ready[i] = _scale[i].is_ready();
    }
}

//=========================================================
// Mittelwertbildung
//=========================================================

long ScaleManager::readAverage(
        uint8_t pot,
        uint8_t samples)
{
    if (pot >= NUMBER_OF_POTS)
        return 0;

    long sum = 0;
    uint8_t count = 0;

    for (uint8_t i = 0; i < samples; i++)
    {
        if (_scale[pot].is_ready())
        {
            sum += _scale[pot].read();

            count++;
        }

        delay(5);
    }

    if (count == 0)
        return 0;

    return sum / count;
}

//=========================================================
// Update aller Waagen
//=========================================================

void ScaleManager::update()
{
    for (uint8_t pot = 0;
         pot < NUMBER_OF_POTS;
         pot++)
    {
        _ready[pot] =
            _scale[pot].is_ready();

        if (!_ready[pot])
            continue;

        _raw[pot] =
            readAverage(
                pot,
                SCALE_AVERAGE_SAMPLES);

        _weight[pot] =
            (_raw[pot] - _offset[pot])
            / _calibration[pot];
    }
}

//=========================================================
// Status
//=========================================================

bool ScaleManager::isReady(
        uint8_t pot) const
{
    if (pot >= NUMBER_OF_POTS)
        return false;

    return _ready[pot];
}
//=========================================================
// Rohwert
//=========================================================

long ScaleManager::getRaw(uint8_t pot) const
{
    if (pot >= NUMBER_OF_POTS)
        return 0;

    return _raw[pot];
}

//=========================================================
// Gewicht
//=========================================================

float ScaleManager::getWeight(uint8_t pot) const
{
    if (pot >= NUMBER_OF_POTS)
        return 0.0f;

    return _weight[pot];
}

//=========================================================
// Tara
//=========================================================

void ScaleManager::tare(uint8_t pot)
{
    if (pot >= NUMBER_OF_POTS)
        return;

    if (!_scale[pot].is_ready())
        return;

    _offset[pot] =
        readAverage(
            pot,
            SCALE_TARE_SAMPLES);

    _raw[pot] = _offset[pot];

    _weight[pot] = 0.0f;
}

//=========================================================
// Offset setzen
//=========================================================

void ScaleManager::setOffset(
        uint8_t pot,
        long value)
{
    if (pot >= NUMBER_OF_POTS)
        return;

    _offset[pot] = value;
}

//=========================================================
// Offset lesen
//=========================================================

long ScaleManager::getOffset(
        uint8_t pot) const
{
    if (pot >= NUMBER_OF_POTS)
        return 0;

    return _offset[pot];
}

//=========================================================
// Kalibrierfaktor setzen
//=========================================================

void ScaleManager::setCalibration(
        uint8_t pot,
        float factor)
{
    if (pot >= NUMBER_OF_POTS)
        return;

    if (factor == 0.0f)
        return;

    _calibration[pot] = factor;
}

//=========================================================
// Kalibrierfaktor lesen
//=========================================================

float ScaleManager::getCalibration(
        uint8_t pot) const
{
    if (pot >= NUMBER_OF_POTS)
        return 0.0f;

    return _calibration[pot];
}