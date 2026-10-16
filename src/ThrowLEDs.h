#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>

class ThrowLEDs{
    public:
        ThrowLEDs(Adafruit_PWMServoDriver* servos);        
        void begin();
        void throwOne();            //illuminate 1st LED @100%
        void throwTwo();            //illuminate both LEDs @100%
        void clear();               //turn off both LEDs
    
    private:
        Adafruit_PWMServoDriver* _servoInterface;
};