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
            INIT,
            //STARTMENU,
            CLEARTURRET,
            SWEEPGUARDTOBACK,
            SWEEPBACKTOGUARD,    
            SWEEPCOMPLETE,
            WAITFORPINS,
            DROPNEWPINS,
            PINSDROPPED,
            RAISEDECK,
            THROWTWO,
            SWEEPUP,
            NEARLYREADY,  
            READY,
            THROWONE,
            SWEEPUPTOGUARD,
            CLOSESCISSORS,
            LIFTPINS,
            PINSLIFTED,
            PINSREPLACING,
            PINSREPLACED,
            CYCLE,
            DELAYCYCLE,
            GETPINS
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
            bool throwOne;
            bool motorRunning;
        };

        void begin(FastAccelStepperEngine* engine);                   //initialises the pinsetter
        void heartbeat();               //call periodically for sweep tweens etc. This is passed to other objects.
        
        void turretHomed();             //called by sensor interrupt from main program 
        void pinSense();                //called by sensor interrupt from main program
        bool doCycle(bool pause);       //trigger the pinsetter cycle. Set pause= true to include a delay.
            
        bool startMotor();           //restart the coneyor if stopped.      
        
        pinsetterStatus getStatus();    //returns a struct of current pinsetter status
    
    private:        
        PinsetterPose currentPose = PinsetterPose::INIT;    
        PinsetterMode currentMode = PinsetterMode::INITIALISING1;
        Adafruit_PWMServoDriver* _servoInterface;
        FastAccelStepperEngine* _engine;
        Deck _deck;
        Sweep _sweep;
        BallReturn _ballReturn;
        MainMotor _mainMotor;
        ThrowLEDs _throwLEDs;
        DeckLighting _deckLighting;
        Turret _turret;
        uint8_t delaySteps = 0;            
    };