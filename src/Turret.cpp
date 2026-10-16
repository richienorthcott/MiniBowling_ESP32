#include <Turret.h>

Turret::Turret(MoToStepper* stepper){
    _stepper = stepper;
    currentPose = TurretPose::STARTUP;
}

void Turret::begin(){
    deckHasPins = true;//use this to clear the deck on startup
    if(!digitalRead(TURRET_HOME_PIN)){
        _stepper->stop();
        _stepper->setZero();
        _stepper->setSpeedSteps(TURRET_SPEED_RUN);
        _stepper->setRampLen(TURRET_ACCEL);
        _stepper->moveTo(TURRET_MOVE_OFF_SENSOR);
        currentPose = TurretPose::MOVEOFFHOME;
    }else{
        currentPose = TurretPose::HOMING;
        _stepper->setSpeedSteps(TURRET_SPEED_HOMING);
        _stepper->rotate(-1);
    }
}

void Turret::homeSense(){
    if(currentPose == TurretPose::HOMING){
        _stepper->stop();
        while(_stepper->moving());    
        _stepper->setZero();
        _stepper->setSpeedSteps(TURRET_SPEED_RUN);
        _stepper->setRampLen(TURRET_ACCEL);
        _stepper->moveTo(TURRET_HOME_OFFSET);
        currentPose = TurretPose::MOVETOSLOT0;
    }
}

void Turret::heartbeat(){    
    switch(currentPose){
        case TurretPose::MOVEOFFHOME:
            if(_stepper->distanceToGo() == 0){
                begin();
            }
            break;
        case TurretPose::MOVETOSLOT0:
            if(_stepper->distanceToGo() == 0){
                _stepper->setZero();
                currentPose = TurretPose::RECEIVEPINS;
                currentSlot = 0;
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
            _stepper->moveTo(TURRET_POSITION1);
            break;    
        case 2:
            _stepper->moveTo(TURRET_POSITION2);
            break;
        case 3:
            _stepper->moveTo(TURRET_POSITION3);
            break;
        case 4:
            _stepper->moveTo(TURRET_POSITION4);
            break;
        case 5:
            _stepper->moveTo(TURRET_POSITION5);
            break;
        case 6:
            _stepper->moveTo(TURRET_POSITION6);
            break;
        case 7:
            _stepper->moveTo(TURRET_POSITION7);
            break;
        case 8:
            _stepper->moveTo(TURRET_POSITION8);
            break;
        case 9:
            _stepper->moveTo(TURRET_POSITION9);
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
            _stepper->moveTo(TURRET_POSITION1);
            break;    
        case 2:
            _stepper->moveTo(TURRET_POSITION2);
            break;
        case 3:
            _stepper->moveTo(TURRET_POSITION3);
            break;
        case 4:
            _stepper->moveTo(TURRET_POSITION4);
            break;
        case 5:
            _stepper->moveTo(TURRET_POSITION5);
            break;
        case 6:
            _stepper->moveTo(TURRET_POSITION6);
            break;
        case 7:
            _stepper->moveTo(TURRET_POSITION7);
            break;
        case 8:
            _stepper->moveTo(TURRET_POSITION8);
            break;
        case 9:
            _stepper->moveTo(TURRET_POSITION9);
            break;
    }   
}

void Turret::pinSense(){
    delayActive = true;
    delayStarted = millis();
}