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
    if(!di.p0){                 //Pin 1
        retval.pinsDown += 1;
        retval.pins |= 0x1;
    }
    if(!di.p1){                 //Pin 2
        retval.pinsDown += 1;
        retval.pins |= 0x2;
    }
    if(!di.p2){                 //Pin 3
        retval.pinsDown += 1;
        retval.pins |= 0x4;
    }
    if(!di.p3){                 //Pin 4
        retval.pinsDown += 1;
        retval.pins |= 0x8;
    }
    if(!di.p4){                 //Pin 5
        retval.pinsDown += 1;
        retval.pins |= 0x10;
    }
    if(!di.p11){                //Pin 6
        retval.pinsDown += 1;
        retval.pins |= 0x20;
    }
    if(!di.p12){                //Pin 7
        retval.pinsDown += 1;
        retval.pins |= 0x40;
    }
    if(!di.p13){                //Pin 8
        retval.pinsDown += 1;
        retval.pins |= 0x80;
    }
    if(!di.p14){                //Pin 9
        retval.pinsDown += 1;
        retval.pins |= 0x100;
    }
    if(!di.p15){                //Pin 10
        retval.pinsDown += 1;
        retval.pins |= 0x200;
    }
    return retval; 
};