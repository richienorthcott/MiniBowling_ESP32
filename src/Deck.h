#pragma once
#include <Globals.h>
#include <Slide.h>
#include <Scissors.h>
#include <Adafruit_PWMServoDriver.h>

class Deck{
    public:
        enum DeckPose{
            UP,
            DOWN,            
            GRAB,
            SWEEP
        };
        Deck(Adafruit_PWMServoDriver* servos);                
        void begin();
        void setPosition(enum DeckPose pos);        
        DeckPose currentPose = DeckPose::UP;
        Scissors scissors;
        Slide slide;
    private:
        Adafruit_PWMServoDriver* _servoInterface;
        
};