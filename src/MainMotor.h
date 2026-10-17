#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>

class MainMotor{
    public:
        MainMotor(Adafruit_PWMServoDriver* servos);
        
        enum MainMotorPose{
            STOPPED,
            RUNNING
        };
        
        MainMotorPose currentPose = MainMotorPose::STOPPED;

        void begin();
        void heartbeat();        

        void start();                       //start the main motor
        void start(uint8_t percentSpeed);   //start the main motor at reduced speed
        void stop();                        //stop the main motor
        void resetTimeout();                //reset the timeout. This won't restart the motor if it's paused
    
        bool paused = false;                //public property, indicates if motor is in 'paused' state. currentPose will still be RUNNING
    private:
        Adafruit_PWMServoDriver* _servoInterface;
        unsigned long conveyorStarted=0;
};
