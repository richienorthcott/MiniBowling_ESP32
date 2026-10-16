#pragma once
#include <Globals.h>
#include <Adafruit_PWMServoDriver.h>

class BallReturn{
    public:
        BallReturn(Adafruit_PWMServoDriver* servos);
        
        enum BallReturnPose{
            OPEN,
            CLOSED
        };        
        
        BallReturnPose currentPose = CLOSED;
        void begin();
        void openDoor();
        void closeDoor();
        void armDoor(); //schedules an open/close function from ball detection
        void heartbeat();
    private:
        Adafruit_PWMServoDriver* _servoInterface;
        int pulseCount = 0;
        bool isArmed = false;
};