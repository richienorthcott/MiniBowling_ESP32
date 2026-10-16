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
