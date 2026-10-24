#pragma once
#include <Globals.h>
#include <Pindicator.h>
#include <PinSensor.h>


class Scoring{
    public:
        Scoring(PCF8575* sensors, HT16K33* matrix);    
        void begin();
        void heartbeat();

    private:
        Pindicator pindicator;
        PinSensor pinsensor;
};