#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>
#include <Slide.h>

class Scissors{
    public:
        enum ScissorsPose{
            GRAB,
            RELEASE
        };    
    
        Scissors(Adafruit_PWMServoDriver* servos);
        ScissorsPose currentPose = RELEASE;    
        void begin();
        bool grab(enum Slide::SlidePose pose);
        void release();

    private:
        Adafruit_PWMServoDriver* _servoInterface;
};