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
        void heartbeat();

        void openDoor();        //open the door immediately
        void closeDoor();       //close the door immediately
        void armDoor();         //schedules an open/close, controlled by timeouts.

    private:
        Adafruit_PWMServoDriver* _servoInterface;
        int pulseCount = 0;
        bool isArmed = false;
};
