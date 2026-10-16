#include <ThrowLEDs.h>

ThrowLEDs::ThrowLEDs(Adafruit_PWMServoDriver* servos){
    _servoInterface = servos;
}

void ThrowLEDs::begin(){
    this->clear();
}

void ThrowLEDs::throwOne(){
    _servoInterface->setPWM(THROW1_LED_CHANNEL, 0, MAX_PWM);
    _servoInterface->setPWM(THROW2_LED_CHANNEL, 0, 0);
    
}

void ThrowLEDs::throwTwo(){
    _servoInterface->setPWM(THROW1_LED_CHANNEL, 0, MAX_PWM);
    _servoInterface->setPWM(THROW2_LED_CHANNEL, 0, MAX_PWM);
}

void ThrowLEDs::clear(){
    _servoInterface->setPWM(THROW1_LED_CHANNEL, 0, 0);
    _servoInterface->setPWM(THROW2_LED_CHANNEL, 0, 0);
}