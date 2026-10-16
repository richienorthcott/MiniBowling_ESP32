#pragma once
#include <Arduino.h>
#include <PCF8575.h>

class PinSensor{
    public:
        PinSensor(PCF8575* interface);
        void begin();
        struct pinStatus{
            uint16_t pins; //each bit of this number represents a pin, used by the pindicator
            uint8_t score; //this is the score from 0-10
        };
        pinStatus getStatus(); 
    private:
        PCF8575* _interface;      
};