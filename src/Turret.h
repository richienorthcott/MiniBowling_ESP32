#pragma once
#include <FastAccelStepper.h>
#include <pin_config.h>
#include <general_config.h>

class Turret{
    public:
        Turret();

        enum TurretPose{
            STARTUP,
            HOMING,
            MOVEOFFHOME,
            MOVETOSLOT0,
            RECEIVEPINS
        };

        TurretPose currentPose = STARTUP;

        void begin(FastAccelStepperEngine* engine);
        void init();
        void heartbeat();
        
        void homeSense();                //Notify the turret the home sensor has been activated
        void pinSense();                 //Notify the turret a pin is approaching on the conyeyor
        void moveToSlot(uint8_t pin);    //Move ASAP to the specified turret position (0-9)

        bool deckHasPins = false;        //Public property, set to true if the deck has had pins released into it
        uint8_t currentSlot = 0;         //Public property, returns the currently active turret slot
    private:
        FastAccelStepperEngine* _engine;    
        FastAccelStepper* _stepper = NULL;
    
        void advanceTurret();            //Initialise a delay to advance to the next slot
        bool delayActive = false;
        unsigned long delayStarted = 0;
};
