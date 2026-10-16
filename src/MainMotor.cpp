#include <MainMotor.h>

MainMotor::MainMotor(Adafruit_PWMServoDriver* servos){    
    _servoInterface = servos;
}
void MainMotor::begin(){
    stop();
};

void MainMotor::heartbeat(){
    if (millis() - conveyorStarted > CONVEYOR_TIMEOUT_MS && currentPose == MainMotor::MainMotorPose::RUNNING) stop();
};

void MainMotor::resetTimeout(){    
    conveyorStarted = millis();
}

void MainMotor::stop(){     
    _servoInterface->setPWM(CONVEYOR_CHANNEL, 0, 0);
    currentPose = MainMotorPose::STOPPED;
};

void MainMotor::start(){     
    _servoInterface->setPWM(CONVEYOR_CHANNEL, 0, 4095);
    currentPose = MainMotorPose::RUNNING;
    conveyorStarted = millis();
    paused = false;
};

void MainMotor::start(uint8_t percentSpeed){
    int pulseWidth = float(100-percentSpeed) * 40.95;
    _servoInterface->setPWM(CONVEYOR_CHANNEL, 0, pulseWidth);
    paused = false;
};