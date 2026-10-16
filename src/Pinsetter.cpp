#include <Pinsetter.h>

Pinsetter::Pinsetter(Adafruit_PWMServoDriver* servos, MoToStepper* stepper):_deck(servos), _sweep(servos), _ballReturn(servos), _mainMotor(servos), _throwLEDs(servos), _deckLighting(servos), _turret(stepper){
     _servoInterface = servos;
     _stepper = stepper;
}

void Pinsetter::begin(){
     _deck.begin();
     _sweep.begin();
     _ballReturn.begin();
     _mainMotor.begin();
     _throwLEDs.begin();
     _deckLighting.begin();
     _turret.begin();
     currentPose = PinsetterPose::INIT;
     currentMode = PinsetterMode::INITIALISING1;
}

void Pinsetter::turretHomed(){
     _turret.homeSense();
}

void Pinsetter::pinSense(){          
     _turret.pinSense();
     //_mainMotor.resetTimeout(); this function may not be needed!!!
     _mainMotor.start();
}

void Pinsetter::heartbeat(){
     
     //this is the main pinsetter loop, through all the various possible states / functions.
     switch(currentPose){
          
          case PinsetterPose::INIT:
               //wait for turret homing sequence to complete
               if(_turret.currentPose == Turret::TurretPose::RECEIVEPINS) currentPose = PinsetterPose::SWEEPGUARDTOBACK;
               break;          
          
          case PinsetterPose::STARTMENU:
               currentMode = PinsetterMode::THROW_ONE;     
               currentPose = PinsetterPose::SWEEPUP;
               break;
          
          case PinsetterPose::CLEARTURRET:
               delaySteps +=1;
               if(delaySteps == TURRET_FULL_ROTATE){                    
                    _turret.moveToSlot(0);
                    _turret.deckHasPins = true;
                    currentPose = PinsetterPose::WAITFORPINS;                                        
               }
               break;
          
          case PinsetterPose::SWEEPGUARDTOBACK:
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::BACK, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = Pinsetter::SWEEPBACKTOGUARD;
               }
               break;
          
          case PinsetterPose::SWEEPBACKTOGUARD:
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::GUARD, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = Pinsetter::SWEEPCOMPLETE;
               }
               break;
          
          case PinsetterPose::SWEEPCOMPLETE:
               if(!_sweep.isAnimating){
                    switch(currentMode){
                         case PinsetterMode::INITIALISING1:
                              currentMode = PinsetterMode::INITIALISING2;
                              currentPose = PinsetterPose::THROWTWO;
                              break;
                         case PinsetterMode::THROW_TWO:
                              _throwLEDs.clear();
                              currentPose = PinsetterPose::WAITFORPINS;       
                              break;
                         case PinsetterMode::THROW_ONE:
                              currentPose = PinsetterPose::PINSREPLACING;
                              _deck.setPosition(Deck::DeckPose::GRAB);
                              delaySteps = 0;
                              break;
                    }         
               }
               break;

          case PinsetterPose::WAITFORPINS:
               if(_turret.deckHasPins){
                    _deck.setPosition(Deck::DeckPose::DOWN);
                    currentPose = Pinsetter::DROPNEWPINS;
                    delaySteps = 0;
               }
               break;          
          
          case PinsetterPose::DROPNEWPINS:
               delaySteps += 1;
               if(delaySteps == DECK_DOWN_TO_UP){
                    _deck.slide.setPosition(Slide::SlidePose::DUMP);
                    _turret.deckHasPins = false;
                    currentPose = PinsetterPose::PINSDROPPED;
                    delaySteps = 0;
               }
               break;
          
          case PinsetterPose::PINSDROPPED:
               delaySteps += 1;
               if(delaySteps == PIN_RELEASE){
                    _deck.setPosition(Deck::DeckPose::UP);
                    currentPose = PinsetterPose::RAISEDECK;
                    delaySteps = 0;
               }
               break;

          case PinsetterPose::RAISEDECK:
               delaySteps += 1;
               if(delaySteps == DECK_MID_RISE){
                    _deck.slide.setPosition(Slide::SlidePose::RECEIVE);
                    _mainMotor.paused = false;
                    switch(currentMode){
                         case PinsetterMode::INITIALISING1:
                              currentPose = PinsetterPose::THROWTWO;
                              currentMode = PinsetterMode::INITIALISING2;
                              break;
                         
                         case PinsetterMode::INITIALISING2:
                              _turret.moveToSlot(9);
                              delaySteps = 0;
                              currentPose = PinsetterPose::CLEARTURRET;
                              currentMode = PinsetterMode::INITIALISING3;
                              break;
                         
                         case PinsetterMode::INITIALISING3:
                              _mainMotor.start();
                              _deckLighting.turnOn();
                              currentPose = PinsetterPose::STARTMENU;
                              _throwLEDs.throwOne();
                         
                              break;
                         case PinsetterMode::THROW_ONE:
                              currentMode = THROW_TWO;
                              _throwLEDs.throwTwo();
                              currentPose = PinsetterPose::SWEEPUP;                    
                              break;
                         
                         case PinsetterMode::THROW_TWO:
                              currentMode = THROW_ONE;
                              _throwLEDs.throwOne();     
                              currentPose = PinsetterPose::SWEEPUP;
                              break;
                         }
                    }               
               break;
          
          case PinsetterPose::THROWTWO:
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::GUARD, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = SWEEPGUARDTOBACK;
               }
               break;

          case PinsetterPose::SWEEPUP:
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::UP, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = NEARLYREADY;
               }
               break;
          
          case PinsetterPose::NEARLYREADY:
               if(!_sweep.isAnimating){
                    currentPose = PinsetterPose::READY;
               }
               break;
          
          case PinsetterPose::READY:
               break;

          case PinsetterPose::THROWONE:
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::GUARD, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = SWEEPUPTOGUARD;
               }
               break;
          
          case PinsetterPose::SWEEPUPTOGUARD:
               if(!_sweep.isAnimating){
                    _deck.setPosition(Deck::DeckPose::GRAB);
                    delaySteps = 0;
                    currentPose = PinsetterPose::CLOSESCISSORS;
               }
               break;
          
          case PinsetterPose::CLOSESCISSORS:
               delaySteps += 1;
               if(delaySteps == DECK_UP_TO_GRAB){
                    _deck.scissors.grab(_deck.slide.currentPose);
                    currentPose = Pinsetter::LIFTPINS;
                    delaySteps = 0;
               }
               break;

          case PinsetterPose::LIFTPINS:
               delaySteps += 1;
               if(delaySteps == DECK_SCISSORS_OPEN_CLOSE){
                    _deck.setPosition(Deck::DeckPose::SWEEP);                    
                    currentPose = PinsetterPose::PINSLIFTED;;
                    delaySteps = 0;
               }    
               break;
           
           case PinsetterPose::PINSLIFTED:
               delaySteps += 1;              
               if(delaySteps == DECK_UP_SWEEP){
                    currentPose = SWEEPGUARDTOBACK;
                    delaySteps = 0; 
               }               
               break;
          case PinsetterPose::PINSREPLACING:
               delaySteps += 1;              
               if(delaySteps == DECK_SWEEP_TO_GRAB){
                    currentPose = PINSREPLACED;
                    delaySteps = 0; 
               }               
               break;

          case PinsetterPose::PINSREPLACED:
               delaySteps += 1;
               if(delaySteps = DECK_SWEEP_TO_GRAB){
                    _deck.scissors.release();
                    currentPose = PINSDROPPED;
                    delaySteps = 0;
               }
               break;
     }

     if(_turret.currentSlot >= TURRET_PAUSE_SLOT){               
          if(_mainMotor.currentPose == MainMotor::MainMotor::RUNNING){                         
               if(_turret.deckHasPins || _deck.currentPose!=Deck::DeckPose::UP){                                                  
                    _mainMotor.paused = true;
                    _mainMotor.stop();
               }               
          }
               
          if(_mainMotor.paused && _deck.currentPose == Deck::DeckPose::UP && !_turret.deckHasPins){               
               _mainMotor.start();
          }
     }

     //these objects have time related functions, call from here.
     _mainMotor.heartbeat();
     _sweep.heartbeat();
     _turret.heartbeat();
     _ballReturn.heartbeat();
}

bool Pinsetter::doCycle(){
     if(currentPose == PinsetterPose::READY){
          DEBUG_LOG("ball", "break");        
          _ballReturn.armDoor();
          if(!_mainMotor.paused) _mainMotor.start();
          
          switch (currentMode){
               case PinsetterMode::THROW_ONE:
                    currentPose = PinsetterPose::THROWONE;
                    break;
               case PinsetterMode::THROW_TWO:
                    currentPose = PinsetterPose::THROWTWO;
                    break;
               default:
                    return false;
          }
          return true;          
     }
     return false; //don't do anything if current state is not READY     
}

void Pinsetter::startConveyor(){
     _mainMotor.start();
}

void Pinsetter::bypassCycle(bool bypass){
     //response from main loop, indicates if all pins missed
     if(currentMode == PinsetterMode::THROW_ONE){
          if(bypass){
               //currentPose = PinsetterPose::ABORTRESPOT;    
          }else{
               //currentPose = PinsetterPose::CONTINUETHROWONE;
          }
     }else{
          if(bypass){
               //currentPose = PinsetterPose::ABORTTHROWTWO;
          }else{
               //currentPose = PinsetterPose::THROWTWO;
          }
     }
}
     
void Pinsetter::submitStrike(bool strike){
     //response from main loop, indicates if strike on ball 1
     if(strike){
          //currentPose = PinsetterPose::STRIKE; 
     }else{
          //currentPose = PinsetterPose::RAISEDECK;   
     }
}

Pinsetter::pinsetterStatus Pinsetter::getStatus(){
     Pinsetter::pinsetterStatus ret;
     ret.deckLoaded = _turret.deckHasPins;
     ret.turretSlot = _turret.currentSlot;
     ret.operatingMode = currentPose;
     if(currentMode == Pinsetter::PinsetterMode::THROW_TWO){
          ret.throwOne = false;
     }else{
          ret.throwOne = true;
     }
     return ret;
}