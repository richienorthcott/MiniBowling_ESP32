#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>

class Slide{
    public:
        Slide(Adafruit_PWMServoDriver* servos);
        
        enum SlidePose{            
            RECEIVE,
            DUMP
        };        
        
        SlidePose currentPose = RECEIVE;

        void begin();        
        void setPosition(SlidePose pos);
        void dumpPins();
        void receivePins();
    
    private:
        Adafruit_PWMServoDriver* _servoInterface;        
};
