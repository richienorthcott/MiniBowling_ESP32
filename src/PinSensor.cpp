#include <PinSensor.h>

PinSensor::PinSensor(PCF8575* interface){
    _interface = interface;    
}

void PinSensor::begin(){
    for(uint8_t i=0; i<16; i++) _interface->pinMode(i, INPUT);
    _interface->begin();   
}

PinSensor::pinStatus PinSensor::getStatus(){
    PinSensor::pinStatus retval;
    uint16_t pinStatus = 0;
    
    PCF8575::DigitalInput di = _interface->digitalReadAll();
    if(!di.p13){
        retval.pinsDown += 1;
        retval.pins |= 0x80;
    }
    if(!di.p11){
        retval.pinsDown += 1;
        retval.pins |= 0x100;
    }
    if(!di.p12){
        retval.pinsDown += 1;
        retval.pins |= 0x10;
    }
    return retval; 
};