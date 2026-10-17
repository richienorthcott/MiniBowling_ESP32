#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>
#include <Pinsetter.h>
#include <Sweep.h>
#include <Turret.h>
#include <MobaTools.h>
#include <DeckLighting.h>
#include <LaneLighting.h>
#include <FastLED.h>

hw_timer_t *heartbeat = NULL;

Adafruit_PWMServoDriver servos(PCA9685_ADDRESS);
MoToStepper turretStepper(TURRET_STEPS_PER_REV, STEPDIR);
CRGB lane_leds[LANE_PIXEL_COUNT];
Pinsetter pinsetter(&servos, &turretStepper);
LaneLighting laneLighting(lane_leds);

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
    pinsetter.begin();
    laneLighting.solidBlue();
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
        if(pinsetter.doCycle(false))laneLighting.comet();    
        flag_btn_select1 = false;
    }

    if(flag_btn_select2){
        pinsetter.startMotor();
        flag_btn_select2 = false;
    }

    if(flag_btn_select3){
        resetPinsetter();        
        flag_btn_select3 = false;
    }

    if(flag_ballSense){
        if(pinsetter.doCycle(true)) laneLighting.comet();
        flag_ballSense = false;
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

    Wire.begin();    
    delay(20);  
    
    //check if the pinsetter is connected (try its I2C address)
    Wire.beginTransmission(PCA9685_ADDRESS);
    uint8_t error = Wire.endTransmission();
    if(error == 2){
        while(true);
    }

    setupInterrupts();
    setupTimer();  
    turretStepper.attach(STEP_PIN, DIR_PIN);
  
    servos.begin();
    servos.setPWMFreq(SERVO_PWM_FREQUENCY);
    delay(10);

    resetPinsetter();
}

void loop() {       
    handleFlags();
    Pinsetter::pinsetterStatus machineStatus = pinsetter.getStatus();
    bool result;//control panel functions return true/false which show if the screen was actually updated. Will depend on current control panel screen/mode

    if(machineStatus.operatingMode != machineState){                
        machineState = machineStatus.operatingMode;                   

        switch(machineState){            
            case Pinsetter::PinsetterPose::READY:                
                laneLighting.wipeGreen();
                break;            
        }      
    }
}