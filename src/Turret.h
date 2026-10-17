#pragma once
#include <MobaTools.h>
#include <Globals.h>

class Turret{
    public:
        Turret(MoToStepper* stepper);

        enum TurretPose{
            STARTUP,
            MOVEOFFHOME,
            HOMING,
            MOVETOSLOT0,
            RECEIVEPINS
        };

        TurretPose currentPose = STARTUP;

        void begin();
        void heartbeat();
        
        void homeSense();                //Notify the turret the home sensor has been activated
        void pinSense();                 //Notify the turret a pin is approaching on the conyeyor
        void moveToSlot(uint8_t pin);    //Move ASAP to the specified turret position (0-9)

        bool deckHasPins = false;        //Public property, set to true if the deck has had pins released into it
        uint8_t currentSlot = 0;         //Public property, returns the currently active turret slot
    private:
        MoToStepper* _stepper;        
        void advanceTurret();            //Initialise a delay to advance to the next slot
        bool delayActive = false;
        unsigned long delayStarted = 0;
};
