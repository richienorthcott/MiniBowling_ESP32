#include <Scissors.h>

Scissors::Scissors(Adafruit_PWMServoDriver* servos){
    _servoInterface = servos;
}

void Scissors::begin(){
    this->release();
}

void Scissors::release(){
    _servoInterface->setPWM(SCISSORS_CHANNEL, 0, SCISSORS_OPEN);
     currentPose = ScissorsPose::RELEASE;
}

bool Scissors::grab(enum Slide::SlidePose pose){
    if(pose == Slide::SlidePose::RECEIVE){ 
        _servoInterface->setPWM(SCISSORS_CHANNEL, 0, SCISSORS_CLOSE);
        currentPose = ScissorsPose::GRAB;
        return true;
    }else{
        return false;
    }
}

