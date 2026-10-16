#pragma once
#include <Globals.h>
#include <MobaTools.h>
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
        Pinsetter(Adafruit_PWMServoDriver* servos, MoToStepper* stepper);
        
        enum PinsetterPose{
            INIT,
            STARTMENU,
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
            PINSREPLACED
        };
        
        enum PinsetterMode{            
            INITIALISING1,
            INITIALISING2,
            INITIALISING3,
            THROW_ONE,
            THROW_TWO
        };

        struct pinsetterStatus{
            bool deckLoaded;
            uint8_t turretSlot;
            PinsetterPose operatingMode;
            bool throwOne;
        };

        void begin();           //initialises the pinsetter
        void heartbeat();       //call periodically for sweep tweens etc. This is passed to other objects.
        
        void turretHomed();     //called by sensor interrupt from main program 
        void pinSense();        //called by sensor interrupt from main program
        bool doCycle();         //trigger the pinsetter cycle  
        void startConveyor();   //restart the coneyor if stopped.      
        
        pinsetterStatus getStatus();    //returns a struct of current pinsetter status
        
        void bypassCycle(bool bypass);  //callback from main loop. Send true if no pins kocked over
        void submitStrike(bool strike); //callback from main loop. Send true if all pins knocked over on throw 1
    private:        
        PinsetterPose currentPose = PinsetterPose::INIT;    
        PinsetterMode currentMode = PinsetterMode::INITIALISING1;
        Adafruit_PWMServoDriver* _servoInterface;
        MoToStepper* _stepper;
        Deck _deck;
        Sweep _sweep;
        BallReturn _ballReturn;
        MainMotor _mainMotor;
        ThrowLEDs _throwLEDs;
        DeckLighting _deckLighting;
        Turret _turret;
        uint8_t delaySteps = 0;            
    };