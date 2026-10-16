#include <Slide.h>

Slide::Slide(Adafruit_PWMServoDriver* servos){    
    _servoInterface = servos;
}

void Slide::begin(){
    setPosition(currentPose);
}

void Slide::setPosition(SlidePose pos){
    switch(pos){
        case SlidePose::DUMP:
            _servoInterface->setPWM(SLIDE_CHANNEL, 0, SLIDE_DUMP);    
            break;
        default:
            _servoInterface->setPWM(SLIDE_CHANNEL, 0, SLIDE_RECEIVE);        
            break;
    }
    currentPose = pos;
}