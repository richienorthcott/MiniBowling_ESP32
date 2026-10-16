#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>

class DeckLighting{
    public:
        DeckLighting(Adafruit_PWMServoDriver* servos);
        void begin();
        void turnOn();                          //turn on LHS and RHS lights @100%
        void turnOn(bool left, bool right);     //turn on LHS and/or RHS lights @100%
        void turnOff();                         //turn off both lights
    private:
        Adafruit_PWMServoDriver* _servoInterface;
};