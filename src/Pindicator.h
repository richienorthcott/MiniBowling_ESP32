#pragma once
#include <Globals.h>
#include <ht16k33.h>

class Pindicator{
    public:
        Pindicator(HT16K33* servos);                        
        void begin();
        
        void showPinStatus(uint16_t pinStatus);
        void showAll();
        void clear();
        uint8_t randomPins();        
    private:        
        HT16K33* _matrixDriver;
};