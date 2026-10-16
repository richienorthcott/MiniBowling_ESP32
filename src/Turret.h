#pragma once
#include <MobaTools.h>
#include <Globals.h>

class Turret{
    public:
        Turret(MoToStepper* stepper);
        void begin();
        void heartbeat();
        void homeSense();        
        void pinSense();
        void moveToSlot(uint8_t pin);
        enum TurretPose{
            STARTUP,
            MOVEOFFHOME,
            HOMING,
            MOVETOSLOT0,
            RECEIVEPINS
        };
        void advanceTurret();
        bool deckHasPins = false;
        uint8_t currentSlot = 0;
        TurretPose currentPose = STARTUP;
    private:
        MoToStepper* _stepper;
        bool delayActive = false;
        unsigned long delayStarted = 0;
};