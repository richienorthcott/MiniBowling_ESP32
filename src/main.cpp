#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

#include <Deck.h>
#include <ThrowLEDs.h>
#include <MainMotor.h>
#include <LaneLighting.h>
#include <Turret.h>
#include <DeckLighting.h>

hw_timer_t *heartbeat = NULL;        
CRGB lane_leds[86]; 

Adafruit_PWMServoDriver servos(0x40);      
Deck myDeck(&servos);
ThrowLEDs myLEDs(&servos);
MainMotor myMotor(&servos);
LaneLighting myLaneLighting(lane_leds); 
FastAccelStepperEngine engine = FastAccelStepperEngine(); 
Turret myTurret;
DeckLighting myDeckLighting(&servos);


volatile bool flag_updateHeartbeat = false;

void ARDUINO_ISR_ATTR onTimer() {
    flag_updateHeartbeat = true;
}

void setup(){
    Serial.begin(115200);
    Wire.begin();
    delay(20);  
    
    servos.begin();
    servos.setPWMFreq(50); 
    engine.init();

    myDeck.begin();
    myMotor.begin();
    myLEDs.begin();
    myTurret.begin(&engine);
    myDeckLighting.begin();

    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    
    myLaneLighting.solidBlue();

    heartbeat = timerBegin(0, 80, true);// The timer will count in microseconds
    timerAttachInterrupt(heartbeat, &onTimer, true);
    timerAlarmWrite(heartbeat, 33333, true); //30Hz
    timerAlarmEnable(heartbeat);
}

void loop(){
    myMotor.start();
    myDeck.slide.receivePins();
    myDeck.setPosition(Deck::DeckPose::DOWN);
    myLEDs.throwOne();
    myTurret.moveToSlot(5);
    myDeckLighting.turnOn();
    delay(2500);
    
    myDeck.slide.dumpPins();    
    myDeck.setPosition(Deck::DeckPose::UP);
    myLEDs.throwTwo();
    myTurret.moveToSlot(0);
    delay(2500);

    //myMotor.stop();
    myLEDs.clear();
    myDeck.slide.receivePins();
    delay(700);
    
    myDeck.scissors.grab(myDeck.slide.currentPose);
    delay(700);
   
    myDeck.scissors.release();
    delay(700);
    
    
    myDeckLighting.turnOff();
    myMotor.start();
    myDeck.slide.dumpPins();
    delay(700);
}  