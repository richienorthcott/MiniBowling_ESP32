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
    private:        
        HT16K33* _matrixDriver;      
};