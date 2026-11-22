#include <Scoring.h>

Scoring::Scoring(PCF8575* sensors, HT16K33* matrix):pindicator(matrix), pinSensor(sensors){}

void Scoring::begin(){
    pinSensor.begin();
    pindicator.begin();
}

uint8_t Scoring::displayPins(bool isThrowOne){
    PinSensor::pinStatus currentStatus = pinSensor.getStatus();
    pindicator.showPinStatus(currentStatus.pins);
    if(isThrowOne){
        throwOneScore = currentStatus.pinsDown;
        return throwOneScore;
    }else{
        return currentStatus.pinsDown - throwOneScore;
    }
}

uint8_t Scoring::randomPins(){
    return pindicator.randomPins();
}

void Scoring::clearPins(){
    pindicator.clear();
}