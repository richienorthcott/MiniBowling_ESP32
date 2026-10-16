#include <Sweep.h>

Sweep::Sweep(Adafruit_PWMServoDriver* servos){
    _servoInterface = servos;
}

void Sweep::begin(){
    moveTo(currentPose, Deck::DeckPose::UP);    
}

bool Sweep::checkPotentialCollision(enum Deck::DeckPose deckPos){    
    //check deck is not in DOWN or GRAB position. These could cause a collision.
    if(deckPos == Deck::DeckPose::DOWN || deckPos == Deck::DeckPose::GRAB){
        return true;
    }
    return false;
}

bool Sweep::moveTo(enum SweepPose pos, enum Deck::DeckPose deckPos){    
    switch(pos){
        case SweepPose::UP:            
            _servoInterface->setPWM(SWEEP_LEFT_CHANNEL, 0, SWEEP_LEFT_PULSEWIDTH_UP);    
            _servoInterface->setPWM(SWEEP_RIGHT_CHANNEL, 2000, 2000 + SWEEP_RIGHT_PULSEWIDTH_UP);    
            currentLeft = SWEEP_LEFT_PULSEWIDTH_UP;
            currentRight = SWEEP_RIGHT_PULSEWIDTH_UP;
            currentPose = SweepPose::UP;
            break;
        case SweepPose::GUARD:            
            if(currentPose == SweepPose::BACK){
                //motion is from back to guard
                if(checkPotentialCollision(deckPos))return false;
            }
            _servoInterface->setPWM(SWEEP_LEFT_CHANNEL, 0, SWEEP_LEFT_PULSEWIDTH_GUARD);    
            _servoInterface->setPWM(SWEEP_RIGHT_CHANNEL, 2000, 2000 + SWEEP_RIGHT_PULSEWIDTH_GUARD);            
            currentLeft = SWEEP_LEFT_PULSEWIDTH_GUARD;
            currentRight = SWEEP_RIGHT_PULSEWIDTH_GUARD;
            currentPose = SweepPose::GUARD;
            break;
        case SweepPose::BACK:
            if(checkPotentialCollision(deckPos))return false;
            _servoInterface->setPWM(SWEEP_LEFT_CHANNEL, 0, SWEEP_LEFT_PULSEWIDTH_BACK);    
            _servoInterface->setPWM(SWEEP_RIGHT_CHANNEL, 2000, 2000 + SWEEP_RIGHT_PULSEWIDTH_BACK);    
            currentLeft = SWEEP_LEFT_PULSEWIDTH_BACK;
            currentRight = SWEEP_RIGHT_PULSEWIDTH_BACK;
            currentPose = SweepPose::BACK;
            break;
    }
    return true;
}

bool Sweep::moveTo(enum SweepPose pos, enum Deck::DeckPose deckPos, int steps){ 
    targetSteps = steps;
    switch(pos){
        case SweepPose::UP:            
            targetLeft = SWEEP_LEFT_PULSEWIDTH_UP;
            targetRight = SWEEP_RIGHT_PULSEWIDTH_UP;
            targetPose = SweepPose::UP;
            break;
        case SweepPose::GUARD:
            if(currentPose == SweepPose::BACK){
                //motion is from back to guard
                if(checkPotentialCollision(deckPos))return false;
            }    
            targetLeft = SWEEP_LEFT_PULSEWIDTH_GUARD;
            targetRight = SWEEP_RIGHT_PULSEWIDTH_GUARD;
            targetPose = SweepPose::GUARD;
            break;
        case SweepPose::BACK:
            if(checkPotentialCollision(deckPos))return false;    
            targetLeft = SWEEP_LEFT_PULSEWIDTH_BACK;
            targetRight = SWEEP_RIGHT_PULSEWIDTH_BACK;
            targetPose = SweepPose::BACK;
            break;        
    }
    
    tweenIncrementLeft = float(targetLeft - currentLeft) / steps;    
    tweenIncrementRight = float(targetRight - currentRight) / steps;    
    currentStep = 0;
    isAnimating = true;
    return true;
}

void Sweep::heartbeat(){
    if(isAnimating){
        doTween();   
    }
}

void Sweep::doTween(){
    if(currentStep != targetSteps){
        targetLeft = currentLeft + tweenIncrementLeft * currentStep;
        targetRight = currentRight + tweenIncrementRight * currentStep;
        
        _servoInterface->setPWM(SWEEP_LEFT_CHANNEL, 0, targetLeft);
        _servoInterface->setPWM(SWEEP_RIGHT_CHANNEL, 2000, 2000 + targetRight);   
    }else{
        //motion is complete!
        currentLeft = targetLeft;
        currentRight = targetRight;
        _servoInterface->setPWM(SWEEP_LEFT_CHANNEL, 0, currentLeft);
        _servoInterface->setPWM(SWEEP_RIGHT_CHANNEL, 2000, 2000 + currentRight);
        isAnimating = false;
        currentPose = targetPose;
    }
    currentStep += 1;
}