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
    private:        
        HT16K33* _matrixDriver;
};