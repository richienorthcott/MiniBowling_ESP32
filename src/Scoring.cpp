#include <Scoring.h>

Scoring::Scoring(PCF8575* sensors, HT16K33* matrix):pindicator(matrix), pinsensor(sensors){}

void Scoring::begin(){
    pinsensor.begin();
    pindicator.begin();
}

void Scoring::heartbeat(){
    pindicator.heartbeat();
}