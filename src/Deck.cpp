#include <Deck.h>

Deck::Deck(Adafruit_PWMServoDriver* servos):scissors(servos),slide(servos){
    _servoInterface = servos;
}

void Deck::begin(){
    setPosition(DeckPose::UP);
    scissors.begin();
    slide.begin();
}

void Deck::setPosition(enum DeckPose pos){
    currentPose = pos;
    switch(pos){
        case DeckPose::UP:            
            _servoInterface->setPWM(RAISE_LEFT_CHANNEL, 0, DECK_LEFT_PULSEWIDTH_UP);
            _servoInterface->setPWM(RAISE_RIGHT_CHANNEL, 2000, 2000 + DECK_RIGHT_PULSEWIDTH_UP);
            break;        
        case DeckPose::SWEEP:
            _servoInterface->setPWM(RAISE_LEFT_CHANNEL, 0, DECK_LEFT_PULSEWIDTH_SWEEP);
            _servoInterface->setPWM(RAISE_RIGHT_CHANNEL, 2000, 2000 + DECK_RIGHT_PULSEWIDTH_SWEEP);
            break;
        case DeckPose::GRAB:
            _servoInterface->setPWM(RAISE_LEFT_CHANNEL, 0, DECK_LEFT_PULSEWIDTH_GRAB);
            _servoInterface->setPWM(RAISE_RIGHT_CHANNEL, 2000, 2000 + DECK_RIGHT_PULSEWIDTH_GRAB);
            break;    
        case DeckPose::DOWN:                
            _servoInterface->setPWM(RAISE_LEFT_CHANNEL, 0, DECK_LEFT_PULSEWIDTH_DOWN);
            _servoInterface->setPWM(RAISE_RIGHT_CHANNEL, 2000, 2000 + DECK_RIGHT_PULSEWIDTH_DOWN);
            break;
    }
};