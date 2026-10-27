#include <DeckLighting.h>

DeckLighting::DeckLighting(Adafruit_PWMServoDriver* servos){
    _servoInterface = servos;    
}

void DeckLighting::begin(){
    turnOff();
}

void DeckLighting::turnOn(){
     _servoInterface->setPWM(DECK_LEFT_LED_CHANNEL, 0, MAX_PWM);
     _servoInterface->setPWM(DECK_RIGHT_LED_CHANNEL, 0, MAX_PWM);
}

void DeckLighting::turnOn(bool left, bool right){
    left ? _servoInterface->setPWM(DECK_LEFT_LED_CHANNEL, 0, MAX_PWM) : _servoInterface->setPWM(DECK_LEFT_LED_CHANNEL, 0, 0);
    right ? _servoInterface->setPWM(DECK_RIGHT_LED_CHANNEL, 0, MAX_PWM) : _servoInterface->setPWM(DECK_RIGHT_LED_CHANNEL, 0, 0);    
} 

void DeckLighting::turnOff(){
     _servoInterface->setPWM(DECK_LEFT_LED_CHANNEL, 0, 0);
     _servoInterface->setPWM(DECK_RIGHT_LED_CHANNEL, 0, 0); 
}