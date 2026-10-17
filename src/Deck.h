#pragma once
#include <Globals.h>
#include <Slide.h>
#include <Scissors.h>
#include <Adafruit_PWMServoDriver.h>

class Deck{
    public:
        Deck(Adafruit_PWMServoDriver* servos);                
        
        enum DeckPose{
            UP,
            DOWN,            
            GRAB,
            SWEEP
        };
        DeckPose currentPose = DeckPose::UP;

        void begin();                           
        void setPosition(enum DeckPose pos);    //sets deck position to the specified DeckPose ENUM    
        
        Scissors scissors;                      //this is a public object, so can be accessed from the pinsetter's code
        Slide slide;                            //this is a public object, so can be accessed from the pinsetter's code
    private:
        Adafruit_PWMServoDriver* _servoInterface;      
};
