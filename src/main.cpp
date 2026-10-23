#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastAccelStepper.h>
#include <LiquidCrystal_I2C.h>
#include <ht16k33.h>
#include <LaneLighting.h>
#include <FastLED.h>
#include <PCF8575.h>

#include <Pinsetter.h>
#include <ControlPanel.h>
#include <Pindicator.h>
#include <PinSensor.h>

hw_timer_t *heartbeat = NULL;

PCF8575 pinSensors(PCF8575_ADDRESS);
Adafruit_PWMServoDriver servos(PCA9685_ADDRESS);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
HT16K33 ledmatrixdriver;
CRGB lane_leds[LANE_PIXEL_COUNT];
FastAccelStepperEngine engine = FastAccelStepperEngine();

Pinsetter pinsetter(&servos);
LaneLighting laneLighting(lane_leds);
ControlPanel controlPanel(&lcd, &ledmatrixdriver);
PinSensor sensorArray(&pinSensors);

Pinsetter::PinsetterPose machineState = Pinsetter::PinsetterPose::INIT;

volatile bool flag_updateHeartbeat, flag_btn_home, flag_btn_reset, flag_btn_select1, flag_btn_select2, flag_btn_select3, flag_ballSense, flag_ballSpeed, flag_pinSense, flag_homeSense, flag_maintenance_on, flag_maintenance_off = false;
volatile unsigned long lastHome, lastReset, lastSelect1, lastSelect2, lastSelect3, lastBallSense, lastBallSpeed, lastPinSense, lastHomeSense = 0;
volatile bool toggleReset = false;

//these vars are passed to the LCD display
uint8_t currentTurretSlot = 0;
bool deckIsLoaded = false;
bool motorRunning = false;
//ControlPanel::ControlPanelPage currentPage = ControlPanel::ControlPanelPage::INIT;

void ARDUINO_ISR_ATTR onTimer() {
    flag_updateHeartbeat = true;
}

void ARDUINO_ISR_ATTR onResetButton(){
    if(millis() - lastReset > DEBOUNCE_MS){
        if(!digitalRead(BUTTON_RESET)){
            lastReset = millis();
            if(toggleReset){
                flag_maintenance_on = true;
                digitalWrite(EN_PIN, LOW);
                digitalWrite(LED_RESET, LOW);
                toggleReset = false;
            }else{
                flag_maintenance_off = true;
                digitalWrite(EN_PIN, HIGH);
                digitalWrite(LED_RESET, HIGH);
                toggleReset = true;
            }
        }
    }
}

void ARDUINO_ISR_ATTR onHomeButton(){
    //all the control panel buttons suffered cross-talk, checking pin status resolves this 
    if(millis() - lastHome > DEBOUNCE_MS){
        if(!digitalRead(BUTTON_HOME)){
            lastHome = millis();
            flag_btn_home = true;
        }
    }
}

void ARDUINO_ISR_ATTR onSelect1(){
    //all the control panel buttons suffered cross-talk, checking pin status resolves this 
    if(millis() - lastSelect1 > DEBOUNCE_MS){
        if(!digitalRead(BUTTON_SELECT1)){
            lastSelect1 = millis();
            flag_btn_select1 = true;
       }
    }
}

void ARDUINO_ISR_ATTR onSelect2(){
    //all the control panel buttons suffered cross-talk, checking pin status resolves this 
    if(millis() - lastSelect2 > DEBOUNCE_MS){
        if(!digitalRead(BUTTON_SELECT2)){
            lastSelect2 = millis();
            flag_btn_select2 = true;
        }
    }
}
 
void ARDUINO_ISR_ATTR onSelect3(){
    //all the control panel buttons suffered cross-talk, checking pin status resolves this 
    if(millis() - lastSelect3 > DEBOUNCE_MS){
        if(!digitalRead(BUTTON_SELECT3)){
            lastSelect3 = millis();
            flag_btn_select3 = true;
        }
    }
}

void ARDUINO_ISR_ATTR onBallSense(){
    if(millis() - lastBallSense > SENSOR_DEBOUNCE_MS){
            lastBallSense = millis();
            flag_ballSense = true;

    }
}

void ARDUINO_ISR_ATTR onBallSpeed(){
    if(millis() - lastBallSpeed > SENSOR_DEBOUNCE_MS){
            lastBallSpeed = millis();
            flag_ballSpeed = true;
    }
}

void ARDUINO_ISR_ATTR onTurretHome(){
    if(millis() - lastHomeSense > DEBOUNCE_MS){
         if(!digitalRead(TURRET_HOME_PIN)){
                lastHomeSense = millis();
                flag_homeSense = true;
        }
    }
}

void ARDUINO_ISR_ATTR onPinSense(){
    if(millis() - lastPinSense > SENSOR_DEBOUNCE_MS){
        lastPinSense = millis();
        flag_pinSense = true;
    }
}

void setupTimer(){
    heartbeat = timerBegin(0, 80, true);// The timer will count in microseconds
    timerAttachInterrupt(heartbeat, &onTimer, true);
    timerAlarmWrite(heartbeat, 33333, true); //30Hz
    timerAlarmEnable(heartbeat);   
}

void setupInterrupts(){
    attachInterrupt(BUTTON_RESET, onResetButton, FALLING);
    attachInterrupt(BUTTON_HOME, onHomeButton, FALLING);
    attachInterrupt(BUTTON_SELECT1, onSelect1, FALLING);
    attachInterrupt(BUTTON_SELECT2, onSelect2, FALLING);
    attachInterrupt(BUTTON_SELECT3, onSelect3, FALLING);
    attachInterrupt(BALL_SENSOR_PIN, onBallSense, FALLING);
    attachInterrupt(BALL_SPEED_PIN, onBallSpeed, FALLING);
    attachInterrupt(IR_SENSOR_PIN, onPinSense, FALLING);
    attachInterrupt(TURRET_HOME_PIN, onTurretHome, FALLING);
}

void setupPins(){
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);    //enable the pinsetter    

    pinMode(LED_RESET, OUTPUT);
    digitalWrite(LED_RESET, LOW); //turn off the reset LED

    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    pinMode(BUTTON_RESET, INPUT);
    pinMode(BUTTON_HOME, INPUT);
    pinMode(BUTTON_SELECT1, INPUT);     
    pinMode(BUTTON_SELECT2, INPUT);
    pinMode(BUTTON_SELECT3, INPUT);
    pinMode(BALL_SENSOR_PIN, INPUT);
    pinMode(BALL_SPEED_PIN, INPUT);
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(TURRET_HOME_PIN, INPUT);
}

void resetPinsetter(){
    pinsetter.begin(&engine);
    controlPanel.mainMenu();
    laneLighting.lavaAnim();
}

void handleFlags(){
   //these flags are set by button interrupts, or main timer loop interrupt. 
    if(flag_updateHeartbeat){
        pinsetter.heartbeat();
        laneLighting.heartbeat();
        flag_updateHeartbeat = false;
    }

    if(flag_maintenance_on){
        laneLighting.turnOn();
        flag_maintenance_on = false;
    }

    if(flag_maintenance_off){
        laneLighting.turnOff();
        flag_maintenance_off = false;
    }

    if(flag_btn_select1){
        switch(controlPanel.currentPose){
            case ControlPanel::ControlPanelPage::MAIN:
                if(pinsetter.doCycle(false)){
                    controlPanel.showSpeed(0);
                    laneLighting.comet();
                }
                break;
            case ControlPanel::ControlPanelPage::UTIL:
                controlPanel.lightMenu();
                break;
            case ControlPanel::ControlPanelPage::LIGHT:                
                laneLighting.rainbowAnim();
                break;
            case ControlPanel::ControlPanelPage::GAME:                                
                controlPanel.scrollGame();    
                break;
            case ControlPanel::ControlPanelPage::SETUP:                
                controlPanel.updatePlayerCount();
                break;
            }
        flag_btn_select1 = false;
    }

    if(flag_btn_select2){
        switch(controlPanel.currentPose){            
            case ControlPanel::ControlPanelPage::MAIN:
                controlPanel.gameMenu();
                break;
            case ControlPanel::ControlPanelPage::UTIL:    
                if (pinsetter.startMotor()) controlPanel.mainMenu();
                break;
            case ControlPanel::ControlPanelPage::LIGHT:                
                laneLighting.strike();
                break;
            case ControlPanel::ControlPanelPage::GAME:                
                break;
            case ControlPanel::ControlPanelPage::SETUP:                
                controlPanel.updateFrameCount();
                break;
            }
        flag_btn_select2 = false;        
    }
     
    if(flag_btn_select3){
        switch(controlPanel.currentPose){
            case ControlPanel::ControlPanelPage::MAIN:
                controlPanel.utilMenu();
                break;
            case ControlPanel::ControlPanelPage::UTIL:
                resetPinsetter();
                break;
            case ControlPanel::ControlPanelPage::LIGHT:                
                laneLighting.wipeGreen();    
                break;
            case ControlPanel::ControlPanelPage::GAME:                
                controlPanel.setupMenu();
                break;
            case ControlPanel::ControlPanelPage::SETUP:                
                controlPanel.initGame();//start a new game                ;    
                break;
            }
        flag_btn_select3 = false;
    }

    if(flag_btn_home){
        if (controlPanel.currentPose != ControlPanel::ControlPanelPage::MAIN) controlPanel.mainMenu();        
        flag_btn_home = false;
    }

    if(flag_ballSense){
        if(pinsetter.doCycle(true)) laneLighting.comet();
        flag_ballSense = false;
    }
    
    if(flag_ballSpeed){
        if(lastBallSpeed - lastBallSense < MAX_BALL_SPEED){
            //calcualate the speed. Sensors are 8.5cm apart
            float speed = 85/float(lastBallSpeed - lastBallSense);
            controlPanel.showSpeed(speed);
        }else{
            //this is probably the sweep blocking the speed sensor
        }
        flag_ballSpeed = false;
    }

    if(flag_homeSense){
        pinsetter.turretHomed();
        flag_homeSense=false;
    }

    if(flag_pinSense){
        pinsetter.pinSense();
        flag_pinSense=false;
    }
}

void setup() {    
    Serial.begin(SERIAL_BAUD); //init serial comms
    while(!Serial);

    setupPins();
    
    controlPanel.begin();
    Wire.begin();    
    delay(20);  
    
    //check if the pinsetter is connected (try its I2C address)
    Wire.beginTransmission(PCA9685_ADDRESS);
    uint8_t error = Wire.endTransmission();
    if(error == 2){
        controlPanel.notConnected();
        while(true);
    }

    setupInterrupts();
    setupTimer();  
  
    engine.init();

    servos.begin();
    servos.setPWMFreq(SERVO_PWM_FREQUENCY);
    delay(10);
    sensorArray.begin();
    delay(3000);
    resetPinsetter();
}

void loop() {       
    handleFlags();
    Pinsetter::pinsetterStatus machineStatus = pinsetter.getStatus();
    bool result;//control panel functions return true/false which show if the screen was actually updated. Will depend on current control panel screen/mode

    if(machineStatus.motorRunning != motorRunning){
        //send the current slot to the LCD display. This is worth doing always as it's cached if not actively on screen
        result = controlPanel.showMotor(machineStatus.motorRunning );
        motorRunning = machineStatus.motorRunning ;
    }

    if(machineStatus.turretSlot != currentTurretSlot){
        //send the current slot to the LCD display. This is worth doing always as it's cached if not actively on screen
        result = controlPanel.showSlot(machineStatus.turretSlot );
        currentTurretSlot = machineStatus.turretSlot ;
    }

    if(machineStatus.deckLoaded != deckIsLoaded){
        //send the current deck loaded true/false to the LCD display. This is worth doing always as it's cached if not actively on screen
        result = controlPanel.showDeck(machineStatus.deckLoaded );
        deckIsLoaded = machineStatus.deckLoaded ;
    }     
    
    //check for change in pinsetter machine state
    if(machineStatus.sequenceStep != machineState){                
        machineState = machineStatus.sequenceStep;               
        result = controlPanel.showPinsetterStatus(machineState);

        switch(machineState){            
            case Pinsetter::PinsetterPose::READY:
                if(machineStatus.throwOne){
                    result = controlPanel.showMessage(ControlPanel::ControlPanelMessage::BOWL1);   
                }else{
                    result = controlPanel.showMessage(ControlPanel::ControlPanelMessage::BOWL2);
                }
                laneLighting.wipeGreen();
                break;
            case Pinsetter::PinsetterPose::WAITFORPINS:
                result = controlPanel.showMessage(ControlPanel::ControlPanelMessage::WAIT);
                break;
            case Pinsetter::PinsetterPose::GETPINS:
                //process the score here
                //if all pins have been kocked down and we'r on throw 1, can advance straight to throw two                    
                pinsetter.doCycle(false);
                break;                        
            default:
                result = controlPanel.showMessage(ControlPanel::ControlPanelMessage::CYCLING);
                break;
        }      
    }
}