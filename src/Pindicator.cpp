#include <Pindicator.h>

Pindicator::Pindicator(HT16K33* driver){
    _matrixDriver = driver;
}

void Pindicator::begin(){
    _matrixDriver->begin(PINDICATOR_ADDRESS);
    _matrixDriver->setBrightness(PINDICATOR_INTENSITY);
    this->showAll();
}

void Pindicator::showAll(){;
  this->showPinStatus(65535);//tutrn on all pin LEDs
}

void Pindicator::heartbeat(){
    delaySteps += 1;
    if(delaySteps == 15){
        delaySteps = 0;
        showPins();
        testVal +=1;
        if (testVal > 10) testVal = 0;
        
    }
}

void Pindicator::showPinStatus(uint16_t pinStatus){
    //each of the first 10 bits in the 16 bit pinStatus values represents an LED state  
    this->clear();
  if((pinStatus & 0x1) == 0x1)_matrixDriver->setLedNow(50);    //pin 1
  if((pinStatus & 0x2) == 0x2)_matrixDriver->setLedNow(49);    //pin 2
  if((pinStatus & 0x4) == 0x4)_matrixDriver->setLedNow(0);     //pin 3
  if((pinStatus & 0x8) == 0x8)_matrixDriver->setLedNow(48);    //pin 4
  if((pinStatus & 0x10) == 0x10)_matrixDriver->setLedNow(3);   //pin 5
  if((pinStatus & 0x20) == 0x20)_matrixDriver->setLedNow(2);   //pin 6
  if((pinStatus & 0x40) == 0x40)_matrixDriver->setLedNow(64);  //pin 7
  if((pinStatus & 0x80) == 0x80)_matrixDriver->setLedNow(65);  //pin 8
  if((pinStatus & 0x100) == 0x100)_matrixDriver->setLedNow(66);//pin 9
  if((pinStatus & 0x200) == 0x200)_matrixDriver->setLedNow(67);//pin 10
};

void Pindicator::clear(){
    _matrixDriver->clearAll();
};

void Pindicator::showPins(){
    switch(testVal){
        case 0:
            showPinStatus(0);
            break;
        case 1:
            showPinStatus(1);
            break;
        case 2:
            showPinStatus(2);
            break;
        case 3:
            showPinStatus(4);
            break;
        case 4:
            showPinStatus(8);
            break;
        case 5:
            showPinStatus(16);
            break;
        case 6:
            showPinStatus(32);
            break;
        case 7:
            showPinStatus(64);
            break;
        case 8:
            showPinStatus(128);
            break;
        case 9:
            showPinStatus(256);
            break;
        case 10:
            showPinStatus(512);
            break;
    }
}