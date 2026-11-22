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
    PCF8575::DigitalInput di = _interface->digitalReadAll();
    
    !di.p0 ? retval.pins |= 0x1 : retval.pinsDown += 1;
    !di.p1 ? retval.pins |= 0x2 : retval.pinsDown += 1;
    !di.p2 ? retval.pins |= 0x4 : retval.pinsDown += 1;
    !di.p3 ? retval.pins |= 0x8 : retval.pinsDown += 1;
    !di.p4 ? retval.pins |= 0x10 : retval.pinsDown += 1;
    !di.p11 ? retval.pins |= 0x20 : retval.pinsDown += 1;
    !di.p12 ? retval.pins |= 0x40 : retval.pinsDown += 1;
    !di.p13 ? retval.pins |= 0x80 : retval.pinsDown += 1;
    !di.p14 ? retval.pins |= 0x100 : retval.pinsDown += 1;
    !di.p15 ? retval.pins |= 0x200 : retval.pinsDown += 1;
    
    return retval; 
};