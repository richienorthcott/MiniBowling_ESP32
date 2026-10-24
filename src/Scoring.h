#pragma once
#include <Globals.h>
#include <Pindicator.h>
#include <PinSensor.h>


class Scoring{
    public:
        Scoring(PCF8575* sensors, HT16K33* matrix);    
        void begin();                
        uint8_t displayPins();  //lights up pin LEDs and returns score
        uint8_t randomPins();
        void clearPins();       //clears all LEDs
        
    private:        
        Pindicator pindicator;    
        PinSensor pinsensor;
};