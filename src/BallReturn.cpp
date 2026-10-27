#include <BallReturn.h>

BallReturn::BallReturn(Adafruit_PWMServoDriver* servos){
    _servoInterface = servos;
}

void BallReturn::begin(){
    closeDoor();
}

void BallReturn::openDoor(){
    _servoInterface->setPWM(BALL_RETURN_CHANNEL, 0, BALL_DOOR_OPEN);
    currentPose = BallReturnPose::OPEN;
}

void BallReturn::closeDoor(){
    _servoInterface->setPWM(BALL_RETURN_CHANNEL, 0, BALL_DOOR_CLOSE);
     currentPose = BallReturnPose::CLOSED;
}

void BallReturn::armDoor(){
    if(currentPose == BallReturnPose::OPEN) closeDoor();
    pulseCount = 0;
    isArmed = true;
}

void BallReturn::heartbeat(){    
    if(isArmed){
        
        if(pulseCount == BALL_DOOR_DELAY_PULSES){
            openDoor();
        }else if(pulseCount == BALL_DOOR_CLOSE_PULSES){            
            closeDoor();
            isArmed = false;
        }
        pulseCount +=1;
    }
}