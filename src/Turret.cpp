#include <Turret.h>

Turret::Turret(){}

void Turret::begin(FastAccelStepperEngine* engine){
    _engine = engine;
    _engine->init();
    
    _stepper = engine->stepperConnectToPin(STEP_PIN);
    _stepper->setDirectionPin(DIR_PIN);
    _stepper->setSpeedInHz(TURRET_SPEED_HOMING);
    _stepper->setAcceleration(TURRET_ACCEL_HOMING);
    
    //check if the turret homing sensor is already active on startup
    if(!digitalRead(TURRET_HOME_PIN)){
        //move off the homing sensor at slow speed
        _stepper->moveTo(TURRET_MOVE_OFF_SENSOR * TURRET_CCW);
        currentPose = TurretPose::MOVEOFFHOME;
    }else{
        //move CW towards the homing sensor at slow speed
        currentPose = TurretPose::HOMING;
        _stepper->move(TURRET_STEPS_PER_REV * TURRET_CW);
    }

    deckHasPins = true;//use this to clear the deck on startup
}

void Turret::homeSense(){
    //notification from honing sensor
    if(currentPose == TurretPose::HOMING){
        _stepper->forceStopAndNewPosition(TURRET_HOME_OFFSET * TURRET_CW);
        _stepper->moveTo(0, false);
        currentPose = TurretPose::MOVETOSLOT0;
    }
}

void Turret::heartbeat(){    
    switch(currentPose){
        case TurretPose::MOVEOFFHOME:
            if(!_stepper->isRunning()){
                //turret has moved off homing sensor (was active on startup). Now home as normal.
                currentPose = TurretPose::HOMING;
                _stepper->move(TURRET_STEPS_PER_REV * TURRET_CW);
            }
            break;
        case TurretPose::MOVETOSLOT0:
            //homing is complete. set the speed and accel higher from now on
            //set to receive state
            if(!_stepper->isRunning()){
                _stepper->setSpeedInHz(TURRET_SPEED_RUN);
                _stepper->setAcceleration(TURRET_ACCEL_RUN);
                currentPose = TurretPose::RECEIVEPINS;
                currentSlot = 0;
            }
            break;
        case TurretPose::RECEIVEPINS:
            //check if a turret advance is necessary 
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
            _stepper->moveTo(TURRET_POSITION1 * TURRET_CCW);
            break;    
        case 2:
            _stepper->moveTo(TURRET_POSITION2 * TURRET_CCW);
            break;
        case 3:
            _stepper->moveTo(TURRET_POSITION3 * TURRET_CCW);
            break;
        case 4:
            _stepper->moveTo(TURRET_POSITION4 * TURRET_CCW);
            break;
        case 5:
            _stepper->moveTo(TURRET_POSITION5 * TURRET_CCW);
            break;
        case 6:
            _stepper->moveTo(TURRET_POSITION6 * TURRET_CCW);
            break;
        case 7:
            _stepper->moveTo(TURRET_POSITION7 * TURRET_CCW);
            break;
        case 8:
            _stepper->moveTo(TURRET_POSITION8 * TURRET_CCW);
            break;
        case 9:
            _stepper->moveTo(TURRET_POSITION9 * TURRET_CCW);
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
            _stepper->moveTo(TURRET_POSITION1 * TURRET_CCW);
            break;    
        case 2:
            _stepper->moveTo(TURRET_POSITION2 * TURRET_CCW);
            break;
        case 3:
            _stepper->moveTo(TURRET_POSITION3 * TURRET_CCW);
            break;
        case 4:
            _stepper->moveTo(TURRET_POSITION4 * TURRET_CCW);
            break;
        case 5:
            _stepper->moveTo(TURRET_POSITION5 * TURRET_CCW);
            break;
        case 6:
            _stepper->moveTo(TURRET_POSITION6 * TURRET_CCW);
            break;
        case 7:
            _stepper->moveTo(TURRET_POSITION7 * TURRET_CCW);
            break;
        case 8:
            _stepper->moveTo(TURRET_POSITION8 * TURRET_CCW);
            break;
        case 9:
            _stepper->moveTo(TURRET_POSITION9 * TURRET_CCW);
            break;
    }   
}

void Turret::pinSense(){
    //notify the turret that a pin is on the eay. Start timeout.
    //when elapsed, the main heartbeat function will pick up on this and advance the turret one slot.
    delayActive = true;
    delayStarted = millis();
}