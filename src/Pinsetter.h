#pragma once
#include <Globals.h>
#include <FastAccelStepper.h>
#include <Adafruit_PWMServoDriver.h>
#include <Deck.h>
#include <Sweep.h>
#include <BallReturn.h>
#include <MainMotor.h>
#include <ThrowLEDs.h>
#include <DeckLighting.h>
#include <Turret.h>

class Pinsetter{
    public:
        Pinsetter(Adafruit_PWMServoDriver* servos);
        
        enum PinsetterPose{
            CLEARTURRET,
            CLOSESCISSORS,
            CYCLE,            
            DELAYCYCLE,
            DROPNEWPINS,
            INIT,
            LIFTPINS,
            NEARLYREADY,
            PINSDROPPED,
            PINSLIFTED,
            PINSREPLACED,
            PINSREPLACING,
            RAISEDECK,
            READPINSDROPPED,
            READTHROWTWO,
            READY,
            SWEEPBACKTOGUARD,    
            SWEEPCOMPLETE,
            SWEEPGUARDTOBACK,
            SWEEPUP,
            SWEEPUPTOGUARD,
            THROWONE,
            THROWTWO,
            WAITFORPINS
        };
        
        enum PinsetterMode{            
            INITIALISING1,
            INITIALISING2,        
            THROW_ONE,
            THROW_TWO
        };

        struct pinsetterStatus{
            bool deckLoaded;
            uint8_t turretSlot;
            PinsetterPose sequenceStep;
            PinsetterMode pinsetterMode;
            //bool throwOne;
            bool motorRunning;
        };

        void begin(FastAccelStepperEngine* engine);                   //initialises the pinsetter
        void init();
        void heartbeat();               //call periodically for sweep tweens etc. This is passed to other objects.
        
        void turretHomed();             //called by sensor interrupt from main program 
        void pinSense();                //called by sensor interrupt from main program
        bool doCycle(bool pause);       //trigger the pinsetter cycle. Set pause= true to include a delay.         

        bool startMotor();           //restart the coneyor if stopped.      
        
        //callbacks for scoring and pin status display
        bool continuefromThrowOne(bool isStrike);
        bool continuefromThrowTwo();

        pinsetterStatus getStatus();    //returns a struct of current pinsetter status
    
    private:        
        PinsetterPose currentPose = PinsetterPose::INIT;    
        PinsetterMode currentMode = PinsetterMode::INITIALISING1;
        Adafruit_PWMServoDriver* _servoInterface;
        Deck _deck;
        Sweep _sweep;
        BallReturn _ballReturn;
        MainMotor _mainMotor;
        ThrowLEDs _throwLEDs;
        DeckLighting _deckLighting;
        Turret _turret;
        uint8_t delaySteps = 0;            
    };