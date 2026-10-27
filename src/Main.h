#include <Globals.h>
//3rd party library includes
#include <Adafruit_PWMServoDriver.h>
#include <FastAccelStepper.h>
#include <LiquidCrystal_I2C.h>
#include <ht16k33.h>
#include <FastLED.h>
#include <PCF8575.h>
//project includes
#include <Pinsetter.h>
#include <LaneLighting.h>
#include <ControlPanel.h>
#include <Scoring.h>

void setup();                                               //default Arduino function
void loop();                                                //default Arduino function

PCF8575 pinSensors(PCF8575_ADDRESS);                        //instance of PCF8575 16 port IO expander module
Adafruit_PWMServoDriver servos(PCA9685_ADDRESS);            //instance of PCA9685 16ch PWM module 
LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);                  //instance of 20 x 4 LCD display with I2C interface
HT16K33 ledmatrixdriver;                                    //instance of 16 row x 8 column LED matrix
CRGB lane_leds[LANE_PIXEL_COUNT];                           //LED array used by FastLED libary. Arrays are the same LHS and RHS, wired in parallel
FastAccelStepperEngine engine = FastAccelStepperEngine();   //FastAccelStepper instance

Pinsetter pinsetter(&servos);                               //instance of project's Pinsetter class. This invokes other child objects.
LaneLighting laneLighting(lane_leds);                       //instance of project's addressable LED class for lane sides
ControlPanel controlPanel(&lcd);                            //instance of project's control panel and game scoring functionality
Scoring scoring(&pinSensors, &ledmatrixdriver);             //instance of project's pin reading and pin display functions, using the PCF8575 and HT16K33 I2C devices

hw_timer_t *heartbeat = NULL;                               //ESP32 timer instance

//these boolean flags are set by the interrupt handlers
volatile bool flag_updateHeartbeat, flag_btn_home, flag_btn_reset, flag_btn_select1, flag_btn_select2, flag_btn_select3, flag_ballSense, flag_ballSpeed, flag_pinSense, flag_homeSense, flag_maintenance_on, flag_maintenance_off = false;

//every button and sensor has an associated debounce lat activation time
volatile unsigned long lastHome, lastReset, lastSelect1, lastSelect2, lastSelect3, lastBallSense, lastBallSpeed, lastPinSense, lastHomeSense = 0;

//on/off toggle for maintenance button. Button is momentary but needs to give on/off functionality. 
volatile bool toggleReset = false;

//these properties are use in the main loop to detect changes in pinsetter state and relay to the control panel only when something has changed
Pinsetter::PinsetterPose machineState = Pinsetter::PinsetterPose::INIT;     
uint8_t currentTurretSlot = 0;                                              
bool deckIsLoaded = false;                                                  
bool motorRunning = false;                                                  

void setupPins();                       //function for configuring ESP32 IO pins and their initial states
void setupInterrupts();                 //function to configure interrupts
void handleInterruptFlags();            //function to process any pending interrupt flags
void resetPinsetter();                  //resets everything, but maintains the active game and its scores hosted by the control panel.
void setupTimer();                      //intialises the hearbeat timer which is used for timing

void ARDUINO_ISR_ATTR onTimer();        //handler for timer interrupt
void ARDUINO_ISR_ATTR onResetButton();  //handler for reset pushbutton interrupt
void ARDUINO_ISR_ATTR onHomeButton();   //handler for home pushbutton interrupt
void ARDUINO_ISR_ATTR onSelect1();      //handler for select1 pushbutton interrupt
void ARDUINO_ISR_ATTR onSelect2();      //handler for select2 pushbutton interrupt
void ARDUINO_ISR_ATTR onSelect3();      //handler for select3 pushbutton interrupt
void ARDUINO_ISR_ATTR onBallSense();    //handler for ball sensor IR beam interrupt
void ARDUINO_ISR_ATTR onBallSpeed();    //handler for ball speed IR beam interrupt
void ARDUINO_ISR_ATTR onTurretHome();   //handler for turret home hall-effect sensor
void ARDUINO_ISR_ATTR onPinSense();     //handler for IR pin sense interrupt