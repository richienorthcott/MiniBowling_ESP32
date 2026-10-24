#pragma once
#include <Globals.h>
#include <ht16k33.h>

class Pindicator{
    public:
        Pindicator(HT16K33* servos);                        
        void showPinStatus(uint16_t pinStatus);
        void showAll();
        void clear();
        void begin();
        void heartbeat();
    private:        
        HT16K33* _matrixDriver;      
        uint8_t testVal = 0;
        uint8_t delaySteps =0;
        void showPins();
};