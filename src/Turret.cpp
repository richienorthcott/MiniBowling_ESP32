#include <Turret.h>

Turret::Turret(){}

void Turret::begin(FastAccelStepperEngine* engine){
    _engine = engine;
    _engine->init();
    _stepper = engine->stepperConnectToPin(STEP_PIN);
    _stepper->setDirectionPin(DIR_PIN);
    
    _stepper->setSpeedInHz(TURRET_SPEED_HOMING);
    _stepper->setAcceleration(TURRET_ACCEL_HOMING);
        
    deckHasPins = true;//use this to clear the deck on startup
    
    if(!digitalRead(TURRET_HOME_PIN)){
        _stepper->moveTo(TURRET_MOVE_OFF_SENSOR * -1);
        currentPose = TurretPose::MOVEOFFHOME;
    }else{
        currentPose = TurretPose::HOMING;
        _stepper->move(TURRET_STEPS_PER_REV);
    }
}

void Turret::homeSense(){
    if(currentPose == TurretPose::HOMING){
        _stepper->forceStopAndNewPosition(TURRET_HOME_OFFSET);
        _stepper->moveTo(0, false);
        currentPose = TurretPose::MOVETOSLOT0;
    }
}

void Turret::heartbeat(){    
    switch(currentPose){
        case TurretPose::MOVEOFFHOME:
            if(!_stepper->isRunning()){
                 currentPose = TurretPose::HOMING;
                _stepper->move(TURRET_STEPS_PER_REV);
            }
            break;
        case TurretPose::MOVETOSLOT0:
            if(!_stepper->isRunning()){
                _stepper->setSpeedInHz(TURRET_SPEED_RUN);
                _stepper->setAcceleration(TURRET_ACCEL_RUN);
                currentPose = TurretPose::RECEIVEPINS;
                currentSlot = 0;
            }else{
                Serial.println("running");
            }
            break;
        case TurretPose::RECEIVEPINS:
            if(delayActive){
                if(millis() - delayStarted > TURRET_ADVANCE_DELAY_MS){
                    delayActive=false;
                    advanceTurret();
                }    
            }    
            break;
    }
}

void Turret::moveToSlot(uint8_t pin){
    currentSlot = pin;
    switch(pin){
        case 0:
            _stepper->moveTo(0);
            break;
        case 1:
            _stepper->moveTo(TURRET_POSITION1 * -1);
            break;    
        case 2:
            _stepper->moveTo(TURRET_POSITION2 * -1);
            break;
        case 3:
            _stepper->moveTo(TURRET_POSITION3 * -1);
            break;
        case 4:
            _stepper->moveTo(TURRET_POSITION4 * -1);
            break;
        case 5:
            _stepper->moveTo(TURRET_POSITION5 * -1);
            break;
        case 6:
            _stepper->moveTo(TURRET_POSITION6 * -1);
            break;
        case 7:
            _stepper->moveTo(TURRET_POSITION7 * -1);
            break;
        case 8:
            _stepper->moveTo(TURRET_POSITION8 * -1);
            break;
        case 9:
            _stepper->moveTo(TURRET_POSITION9 * -1);
            break;
    }   
};

void Turret::advanceTurret(){
    
    currentSlot +=1;
    
    if(currentSlot > 9){
        deckHasPins = true;
        currentSlot = 0;
    }        

    switch(currentSlot){
        case 0:
            _stepper->moveTo(0);
            break;
        case 1:
            _stepper->moveTo(TURRET_POSITION1 * -1);
            break;    
        case 2:
            _stepper->moveTo(TURRET_POSITION2 * -1);
            break;
        case 3:
            _stepper->moveTo(TURRET_POSITION3 * -1);
            break;
        case 4:
            _stepper->moveTo(TURRET_POSITION4 * -1);
            break;
        case 5:
            _stepper->moveTo(TURRET_POSITION5 * -1);
            break;
        case 6:
            _stepper->moveTo(TURRET_POSITION6 * -1);
            break;
        case 7:
            _stepper->moveTo(TURRET_POSITION7 * -1);
            break;
        case 8:
            _stepper->moveTo(TURRET_POSITION8 * -1);
            break;
        case 9:
            _stepper->moveTo(TURRET_POSITION9 * -1);
            break;
    }   
}

void Turret::pinSense(){
    delayActive = true;
    delayStarted = millis();
}