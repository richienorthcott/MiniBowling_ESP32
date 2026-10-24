#include <Scoring.h>

Scoring::Scoring(PCF8575* sensors, HT16K33* matrix):pindicator(matrix), pinsensor(sensors){}

void Scoring::begin(){
    pinsensor.begin();
    pindicator.begin();
}

uint8_t Scoring::displayPins(){
    pindicator.showAll();
    return 10;
}

uint8_t Scoring::randomPins(){
    return pindicator.randomPins();
}

void Scoring::clearPins(){
    pindicator.clear();
}
