#include "PotManager.h"
#include "ScaleManager.h"
#include "PumpManager.h"

PotManager::PotManager():_pot(0),_scale(nullptr),_pump(nullptr){}

void PotManager::begin(uint8_t potNumber,ScaleManager* scale,PumpManager* pump){
    _pot=potNumber;
    _scale=scale;
    _pump=pump;
}

void PotManager::update(){
    if(!_scale||!_pump) return;

    _status.weight=_scale->getWeight(_pot);

    if(!_status.autoMode)
{
    stopWatering();

    _status.state = PotState::MANUAL;

    return;
}
updateState();

    switch(_status.state){
    case PotState::IDLE:
    
{
   

    
}
            
        
        

    default:
        break;
    }
}

void PotManager::setAutoMode(bool e){_status.autoMode=e;}
bool PotManager::getAutoMode() const{return _status.autoMode;}
void PotManager::setStartWeight(float g){_status.startWeight=g;}
void PotManager::setTargetWeight(float g){_status.targetWeight=g;}
float PotManager::getStartWeight() const{return _status.startWeight;}
float PotManager::getTargetWeight() const{return _status.targetWeight;}
float PotManager::getWeight() const{return _status.weight;}
PotState PotManager::getState() const{return _status.state;}
const PotStatus& PotManager::getStatus() const{return _status;}

const char* PotManager::getStateName() const{
    switch(_status.state){
    case PotState::IDLE:return "IDLE";
    case PotState::WAIT_FOR_DRY:return "WAIT_FOR_DRY";
    case PotState::WATERING:return "WATERING";
    case PotState::TARGET_REACHED:return "TARGET_REACHED";
    case PotState::MANUAL:return "MANUAL";
    case PotState::ERROR_SENSOR:return "ERROR_SENSOR";
    case PotState::ERROR_TIMEOUT:return "ERROR_TIMEOUT";
    case PotState::ERROR_TANK:return "ERROR_TANK";
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
        case PotState::WAIT_FOR_DRY:
case PotState::WATERING:

    if(_status.weight >= _status.targetWeight)
    {
        stopWatering();
    }

    break;
            if(_status.weight < _status.startWeight)
            {
                startWatering();
            }

            break;

        case PotState::TARGET_REACHED:

    stopWatering();

    _status.state = PotState::WAIT_FOR_DRY;

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
    _status.state = PotState::WATERING;
}
}


void PotManager::stopWatering()
{
    if (_status.watering)
    {
        _pump->off(_pot);

        _status.watering = false;
        _status.state = PotState::TARGET_REACHED;
    }
}