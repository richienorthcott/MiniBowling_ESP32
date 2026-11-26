#pragma once
#include <Pindicator.h>
#include <PinSensor.h>

class Scoring{
    public:
        Scoring(PCF8575* sensors, HT16K33* matrix);    
        void begin();                
        uint8_t displayPins(bool isThrowOne);  //lights up pin LEDs and returns score
        void clearPins();       //clears all LEDs
        
    private:        
        Pindicator pindicator;    
        PinSensor pinSensor;
        uint8_t throwOneScore = 0;
};