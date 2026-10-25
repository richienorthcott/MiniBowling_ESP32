#include <Pindicator.h>

Pindicator::Pindicator(HT16K33* driver){
    matrixDriver = driver;
}

void Pindicator::begin(){
    matrixDriver->begin(PINDICATOR_ADDRESS);
    matrixDriver->setBrightness(PINDICATOR_INTENSITY);
    this->showAll();
}

void Pindicator::showAll(){;
  this->showPinStatus(65535);//tutrn on all pin LEDs
}

void Pindicator::showPinStatus(uint16_t pinStatus){
    //each of the first 10 bits in the 16 bit pinStatus values represents an LED state  
    updateLED(50, 0x1, pinStatus);      //pin 1
    updateLED(49, 0x2, pinStatus);      //pin 2
    updateLED(0, 0x4, pinStatus);       //pin 3
    updateLED(48, 0x8, pinStatus);      //pin 4
    updateLED(3, 0x10, pinStatus);      //pin 5
    updateLED(2, 0x20, pinStatus);      //pin 6
    updateLED(64, 0x40, pinStatus);     //pin 7
    updateLED(65, 0x80, pinStatus);     //pin 8
    updateLED(66, 0x100, pinStatus);    //pin 9
    updateLED(67, 0x200, pinStatus);    //pin 10
    matrixDriver->sendLed();
};

void Pindicator::updateLED(uint8_t LEDnumber, uint16_t sensorID, uint16_t sensorValue){
    if((sensorValue & sensorID) == sensorID){
        matrixDriver->setLed(LEDnumber);
    }else{
        matrixDriver->clearLed(LEDnumber);
    }
}

void Pindicator::clear(){
    matrixDriver->clearAll();
};

uint8_t Pindicator::randomPins(){
    uint8_t score = 0;
    uint16_t status = 0;

    if(random(2)){
        score +=1;
        status += 1;
    }

    if(random(2)){
        score +=1;
        status += 2;
    }

    if(random(2)){
        score +=1;
        status += 4;
    }

    if(random(2)){
        score +=1;
        status += 8;
    }

    if(random(2)){
        score +=1;
        status += 16;
    }

    if(random(2)){
        score +=1;
        status += 32;
    }

    if(random(2)){
        score +=1;
        status += 64;
    }

    if(random(2)){
        score +=1;
        status += 128;
    }

    if(random(2)){
        score +=1;
        status += 256;
    }

    if(random(2)){
        score +=1;
        status +=512;
    }
    showPinStatus(status);
    return score;

}