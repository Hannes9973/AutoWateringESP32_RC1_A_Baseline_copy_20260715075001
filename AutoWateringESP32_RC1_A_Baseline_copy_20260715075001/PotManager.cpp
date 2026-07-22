#include "PotManager.h"
#include "ScaleManager.h"
#include "PumpManager.h"
#include "StorageManager.h"

PotManager::PotManager():_pot(0),_scale(nullptr),_pump(nullptr){}

void PotManager::begin(uint8_t potNumber,
                       ScaleManager* scale,
                       PumpManager* pump,
                       StorageManager* storage)
{
    _pot = potNumber;
    _scale = scale;
    _pump = pump;
    _storage = storage;
}

void PotManager::update(){
    if(!_scale||!_pump) return;

    _status.weight=_scale->getWeight(_pot);
if(_status.weight < 0.0f)
{
    stopWatering();

    if(_status.state != PotState::ERROR_SENSOR)
    {
        _status.errorCount++;
    }

    _status.state = PotState::ERROR_SENSOR;

    return;
}
    if(!_status.autoMode)
{
    stopWatering();

    _status.state = PotState::MANUAL;

    return;
}
updateState();

    
    
}

void PotManager::setAutoMode(bool e){_status.autoMode=e;}
bool PotManager::getAutoMode() const{return _status.autoMode;}
void PotManager::setStartWeight(float g){_status.startWeight=g;}
void PotManager::setTargetWeight(float g){_status.targetWeight=g;}
float PotManager::getStartWeight() const{return _status.startWeight;}

float PotManager::getTargetWeight() const{return _status.targetWeight;}
float PotManager::getWeight() const{return _status.weight;}
PotState PotManager::getState() const{return _status.state;}
const PotStatus& PotManager::getStatus() const
{
    return _status;
}

uint32_t PotManager::getErrorCount() const


{
    return _status.errorCount;
}

void PotManager::resetErrorCount()
{
    _status.errorCount = 0;
}

void PotManager::setState(PotState state)
{
    _status.state = state;
}

void PotManager::resetState()
{
    if(_scale)
    {
        _scale->tare(_pot);
        _storage->saveScale(_pot, *_scale);
    }

    _status.state = PotState::WAIT_FOR_DRY;
    _status.errorCount = 0;
    _status.wateringTime = millis();

    _status.weight = _scale ? _scale->getWeight(_pot) : 0.0f;
    _status.wateringStartWeight = _status.weight;
}


const char* PotManager::getStateName() const
{
    
    switch(_status.state){
    case PotState::IDLE:return "IDLE";
    case PotState::WAIT_FOR_DRY:return "WAIT_FOR_DRY";
    case PotState::WATERING:return "WATERING";
    case PotState::TARGET_REACHED:return "TARGET_REACHED";
    case PotState::MANUAL:return "MANUAL";
    case PotState::ERROR_SENSOR:return "ERROR_SENSOR";
    case PotState::ERROR_TIMEOUT:return "ERROR_TIMEOUT";
    
    default:return "UNKNOWN";
    }
}
//-----------------------------------------------------
// State Machine
//-----------------------------------------------------

void PotManager::updateState()
{
    switch(_status.state)
{
    case PotState::IDLE:

    _status.state = PotState::WAIT_FOR_DRY;

    break;
case PotState::WAIT_FOR_DRY:

    if(_status.weight < _status.startWeight)
    {
        startWatering();
    }

    break;

case PotState::WATERING:

    if(_status.weight >= _status.targetWeight)
    {
        stopWatering();
        
    }
    
       
    
    else if(millis() - _status.wateringTime > MAX_PUMP_RUNTIME)
    {
        stopWatering();

        if(_status.state != PotState::ERROR_TIMEOUT)
        {
            _status.errorCount++;
        }

        _status.state = PotState::ERROR_TIMEOUT;
    }

    break;

case PotState::TARGET_REACHED:

    stopWatering();
    _status.state = PotState::WAIT_FOR_DRY;

  


    

    break;
    case PotState::ERROR_SENSOR:

    stopWatering();

    if(_status.weight >= 0.0f)
    {
        _status.state = PotState::WAIT_FOR_DRY;
    }

    break;

case PotState::ERROR_TIMEOUT:

    stopWatering();

    if(millis() - _status.wateringTime > 30000)
    {
        _status.state = PotState::WAIT_FOR_DRY;
    }

    break;


default:
    break;
}
}

void PotManager::startWatering()
{
    if(!_status.watering)
    {
        _pump->on(_pot);

        _status.watering = true;

        _status.wateringTime = millis();
        _status.wateringStartWeight = _status.weight;
    _status.state = PotState::WATERING;
    
}
}


void PotManager::stopWatering()
{
    if(_status.watering)
{
    _pump->off(_pot);

    _status.watering = false;

    _status.wateringCount++;

    _status.state = PotState::TARGET_REACHED;
}
}