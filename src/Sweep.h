#pragma once
#include <Globals.h>
#include <Deck.h>

class Sweep{
    public:
        Sweep(Adafruit_PWMServoDriver* servos);

        enum SweepPose{
            UP,
            GUARD,
            BACK,            
        };
        
        SweepPose currentPose = SweepPose::GUARD;
        SweepPose targetPose = SweepPose::GUARD;
        
        void begin();
        void heartbeat();
        bool moveTo(enum SweepPose pos, enum Deck::DeckPose deckPos);               //moves ASAP to position
        bool moveTo(enum SweepPose pos, enum Deck::DeckPose deckPos, int steps);    //moves to position using motion tween
        bool isAnimating = false;    
    private:
        Adafruit_PWMServoDriver* _servoInterface;        
        int currentLeft;        
        int targetLeft;
        int currentRight;        
        int targetRight;
        
        float tweenIncrementLeft;
        float tweenIncrementRight;
        int targetSteps = 0;
        int currentStep = 0;

        bool checkPotentialCollision(enum Deck::DeckPose deckPos);
        void doTween();
        
};