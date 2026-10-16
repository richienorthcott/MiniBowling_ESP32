#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>

class MainMotor{
    public:
        MainMotor(Adafruit_PWMServoDriver* servos);
        void begin();    
        void start();                       //start the main motor
        void start(uint8_t percentSpeed);   //start the main motor at reduced speed
        void stop();                        //stop the main motor
        void resetTimeout();                //reset the timeout. This won't restart the motor if it's paused
        void heartbeat();        
        enum MainMotorPose{
            STOPPED,
            RUNNING
        };
        
        MainMotorPose currentPose = MainMotorPose::STOPPED;
        bool paused = false;
    private:
        Adafruit_PWMServoDriver* _servoInterface;
        unsigned long conveyorStarted=0;
};