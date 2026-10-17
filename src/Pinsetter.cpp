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
     _mainMotor.resetTimeout();
}

void Pinsetter::heartbeat(){
     /*This is the main part of the pinsetter code
     
     On power on (or calling begin() function again), the pinsetter enters an initialisation routine. The status in maintained in the PinsetterMode ENUM, normal operaiton is:
     INITIALISING1 -> INITIALISING2 -> THROW_ONE -> THROW_TWO-> THROW_ONE -> THROW_TWO-> THROW_ONE -> THROW_TWO etc
     
     The intialisation routine does the following, which ensures the machine is in a 'known' state and also prevents collisions.
     The motor is stopped to prevevnt new pins being added to turret.

     1. moves sweep to GUARD position
     2. homes the turret stepper motor
     3. sweeps the lane
     4. lowers the deck and releases any pins
     5. raises the deck
     6. sweeps the lane
     7. releases any pins from the turret
     8. lowers the deck and releases any pins
     9. raises the deck
     10. sweeps the lane
     11. starts the motor and enters the THROW_ONE state
     
     Then the machine follows normal play sequencing.
     Delays are either frames based (at about 30Hz, using the delaySteps increment, or are based on the sweep moving since it uses a tweening approach therefors we know
     when the motion is 'complete'.
     */
     
     switch(currentPose){
          
          case PinsetterPose::INIT:
               //wait for the turret to complete its homing sequence
               if(_turret.currentPose == Turret::TurretPose::RECEIVEPINS) currentPose = PinsetterPose::THROWTWO;
               break;          
          
          case PinsetterPose::STARTMENU:
               //provides a state the main loop can detect, to start the menu. This is at the end of the initialisation sequence
               currentMode = PinsetterMode::THROW_ONE;     
               currentPose = PinsetterPose::SWEEPUP;
               break;
          
          case PinsetterPose::CLEARTURRET:
               //after a delay (allows time for full rotation to slot 10, dropping the pins), return the turret to slot 0
               delaySteps +=1;
               if(delaySteps == TURRET_FULL_ROTATE){                    
                    _turret.moveToSlot(0);
                    _turret.deckHasPins = true;
                    currentPose = PinsetterPose::WAITFORPINS;                                        
               }
               break;
          
          case PinsetterPose::SWEEPGUARDTOBACK:
               //tween the sweep from current position to BACK
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::BACK, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = Pinsetter::SWEEPBACKTOGUARD;
               }
               break;
          
          case PinsetterPose::SWEEPBACKTOGUARD:
               //tween the sweep from current position to GUARD
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::GUARD, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = Pinsetter::SWEEPCOMPLETE;
               }
               break;
          
          case PinsetterPose::SWEEPCOMPLETE:
               //sweep cycle is complete, decide what to do next.
               if(!_sweep.isAnimating){
                    switch(currentMode){
                         case PinsetterMode::INITIALISING1:
                         case PinsetterMode::THROW_TWO:
                              _throwLEDs.clear();
                              currentPose = PinsetterPose::WAITFORPINS;       
                              break;
                         case PinsetterMode::INITIALISING2:
                              _turret.moveToSlot(9);
                              delaySteps = 0;
                              currentPose = PinsetterPose::CLEARTURRET;
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
               //deck will lower to DOWN position when loaded with pins only.
               if(_turret.deckHasPins){
                    _deck.setPosition(Deck::DeckPose::DOWN);
                    currentPose = Pinsetter::DROPNEWPINS;
                    delaySteps = 0;
               }
               break;          
          
          case PinsetterPose::DROPNEWPINS:               
               //drop the new set of pins onto the deck (move the slide to the DUMP position)
               if(delaySteps == DECK_DOWN_TO_UP){
                    _deck.slide.setPosition(Slide::SlidePose::DUMP);
                    _turret.deckHasPins = false;
                    currentPose = PinsetterPose::PINSDROPPED;
                    delaySteps = 0;
               }
               break;
          
          case PinsetterPose::PINSDROPPED:
               //raise the deack to the UP position
               delaySteps += 1;
               if(delaySteps == PIN_RELEASE){
                    _deck.setPosition(Deck::DeckPose::UP);
                    currentPose = PinsetterPose::RAISEDECK;
                    delaySteps = 0;
               }
               break;

          case PinsetterPose::RAISEDECK:
               //move the slide to the RECEIVE position, decide what to do next
               delaySteps += 1;
               if(delaySteps == DECK_MID_RISE){
                    _deck.slide.setPosition(Slide::SlidePose::RECEIVE);
                    _mainMotor.paused = false;
                    switch(currentMode){
                         case PinsetterMode::INITIALISING1:
                              currentMode = PinsetterMode::INITIALISING2;
                              currentPose = PinsetterPose::SWEEPGUARDTOBACK;
                              break;
                         case PinsetterMode::INITIALISING2:
                              //initialisation is now complete
                              _throwLEDs.throwOne();
                              _mainMotor.start();
                              _deckLighting.turnOn();
                              currentPose = PinsetterPose::STARTMENU;
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
               //this is the entry point for the 2nd ball throw
               //tween the sweep to GUARD position
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::GUARD, _deck.currentPose, TWEEN_FRAMES * 2);
                    currentPose = SWEEPGUARDTOBACK;
               }
               break;

          case PinsetterPose::SWEEPUP:
               //tween the sweep from its current position to the UP position
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::UP, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = NEARLYREADY;
               }
               break;
          
          case PinsetterPose::NEARLYREADY:
               //wait for sweep motion to complete
               if(!_sweep.isAnimating){
                    currentPose = PinsetterPose::READY;
               }
               break;
          
          case PinsetterPose::READY:
               //enter the ready state. In this state, pinsetter will respond to cycle requests from the doCycle() function
               break;

          case PinsetterPose::THROWONE:
               //this is the entry point for throw one.
               //tween the sweep to the GUARD position
               if(!_sweep.isAnimating){
                    _sweep.moveTo(Sweep::SweepPose::GUARD, _deck.currentPose, TWEEN_FRAMES);
                    currentPose = SWEEPUPTOGUARD;
               }
               break;
          
          case PinsetterPose::SWEEPUPTOGUARD:
               //lower the deck to the GRAB position
               if(!_sweep.isAnimating){
                    _deck.setPosition(Deck::DeckPose::GRAB);
                    delaySteps = 0;
                    currentPose = PinsetterPose::CLOSESCISSORS;
               }
               break;
          
          case PinsetterPose::CLOSESCISSORS:
               //close the scissors
               delaySteps += 1;
               if(delaySteps == DECK_UP_TO_GRAB){
                    _deck.scissors.grab(_deck.slide.currentPose);
                    currentPose = Pinsetter::LIFTPINS;
                    delaySteps = 0;
               }
               break;

          case PinsetterPose::LIFTPINS:
               //raise the deck to the SWEEP position
               delaySteps += 1;
               if(delaySteps == DECK_SCISSORS_OPEN_CLOSE){
                    _deck.setPosition(Deck::DeckPose::SWEEP);                    
                    currentPose = PinsetterPose::PINSLIFTED;;
                    delaySteps = 0;
               }    
               break;
           
           case PinsetterPose::PINSLIFTED:
               //launch a sweep cycle
               delaySteps += 1;              
               if(delaySteps == DECK_UP_SWEEP){
                    currentPose = SWEEPGUARDTOBACK;
                    delaySteps = 0; 
               }               
               break;
          
          case PinsetterPose::PINSREPLACING:
               //wait for deck to lower to the GRAB position
               delaySteps += 1;              
               if(delaySteps == DECK_SWEEP_TO_GRAB){
                    currentPose = PINSREPLACED;
                    delaySteps = 0; 
               }               
               break;

          case PinsetterPose::PINSREPLACED:
               //open the scissorc
               delaySteps += 1;
               if(delaySteps = DECK_SWEEP_TO_GRAB){
                    _deck.scissors.release();
                    currentPose = PINSDROPPED;
                    delaySteps = 0;
               }
               break;

           case PinsetterPose::CYCLE:
               //entry point to cycle
               switch(currentMode){
                    case PinsetterMode::THROW_ONE:
                         currentPose = PinsetterPose::THROWONE;
                         break;
                    case PinsetterMode::THROW_TWO:
                         currentPose = PinsetterPose::THROWTWO;
                         break;
               }
               break;

           case PinsetterPose::DELAYCYCLE:
               //sets to the CYCLE state but with dealy. This adds a delay before the sweep moves from UP to GUARD
               delaySteps += 1;
               if(delaySteps == CYCLE_DELAY) currentPose = PinsetterPose::CYCLE;          
               break;
               
     }

     //if the turret is nearly full....
     if(_turret.currentSlot >= TURRET_PAUSE_SLOT){                         
          if(_mainMotor.currentPose == MainMotor::MainMotor::RUNNING){                         
               //this section handles two cases where the motor needs to be paused.
               if(_turret.deckHasPins || _deck.currentPose!=Deck::DeckPose::UP){                                                  
                    _mainMotor.paused = true;
                    _mainMotor.stop();
               }               
          }
          //it's safe to restart the motor in this state
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

bool Pinsetter::doCycle(bool pause){
     if(currentPose == PinsetterPose::READY){
          DEBUG_LOG("ball", "break");        
          _ballReturn.armDoor();
          if(!_mainMotor.paused) _mainMotor.start();
          if(pause == true){
               delaySteps = 0; 
               currentPose = PinsetterPose::DELAYCYCLE;
          }else{
               currentPose = PinsetterPose::CYCLE;
          }
          return true;          
     }
     return false; //current pinsetter state is not READY
}

void Pinsetter::startMotor(){
     _mainMotor.start();
}

Pinsetter::pinsetterStatus Pinsetter::getStatus(){
     Pinsetter::pinsetterStatus ret;
     
     ret.deckLoaded = false;
     if(_turret.deckHasPins) ret.deckLoaded = true; 
     ret.turretSlot = _turret.currentSlot;
     ret.operatingMode = currentPose;
     ret.motorRunning = _mainMotor.currentPose == MainMotor::MainMotorPose::RUNNING;
     ret.throwOne = currentMode == Pinsetter::PinsetterMode::THROW_ONE;
     return ret;
}