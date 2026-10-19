#include <ControlPanel.h>

ControlPanel::ControlPanel(LiquidCrystal_I2C* lcd, HT16K33* ledMatrix):_pindicator(ledMatrix){
    _lcd = lcd;
    _ledMatrix = ledMatrix;
}

void ControlPanel::begin(){
    _lcd->init();
    _lcd->backlight();
    _pindicator.begin();
    currentPose = ControlPanelPage::INIT;
    initMenu();
}

void ControlPanel::initMenu(){
    _lcd->clear();
    _lcd->setCursor(0,0); _lcd->print("    MINI BOWLING    ");
    _lcd->setCursor(0,1); _lcd->print("    ============    ");
    _lcd->setCursor(0,2); _lcd->print("    Please wait...  ");
}

void ControlPanel::mainMenu(){
    bool result;
    currentPose = ControlPanelPage::MAIN;
    _lcd->clear();
    
    _lcd->setCursor(0, 0);_lcd->print("       speed:-.--m/s");
    result = this->showSpeed(lastSpeed);//restore previous value
    result = this->showMessage(lastMessage);

    _lcd->setCursor(0, 2);_lcd->print("seq:   load:? deck:?");    
    result = this-showPinsetterStatus(lastStatus);
    result = this->showSlot(lastLoad);
    result = this->showDeck(lastDeck);
    result = this->showMotor(lastMotor);
    _lcd->setCursor(0, 3);_lcd->print("cycle |motor |util  ");   
}

void ControlPanel::utilMenu(){
    currentPose = ControlPanelPage::UTIL;
    _lcd->clear();
    _lcd->setCursor(0, 0);_lcd->print("Utilities");
    _lcd->setCursor(0, 3);_lcd->print("light |score |reset ");
}

void ControlPanel::lightMenu(){
    currentPose = ControlPanelPage::LIGHT;
    _lcd->clear();
    _lcd->setCursor(0, 0);_lcd->print("LED Lighting");
    _lcd->setCursor(0, 3);_lcd->print("rainbo|strike|wipe  ");
}

void ControlPanel::scoreMenu(){
    currentPose = ControlPanelPage::SCORE;
    _lcd->clear();
    _lcd->setCursor(0, 0);_lcd->print("Score Submit");
    _lcd->setCursor(0, 3);_lcd->print("Zero  |Five  |Ten   ");
}

bool ControlPanel::showMessage(ControlPanel::ControlPanelMessage msg){
    lastMessage = msg;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(0, 1);
    _lcd->print("msg:");
    switch(lastMessage){
        case ControlPanelMessage::BOWL1:
            _lcd->print("Bowl first ball");
            break;
        case ControlPanelMessage::BOWL2:
            _lcd->print("Bowl second ball");
            break;
        case ControlPanelMessage::CYCLING:
            _lcd->print("Processing      ");
            break;
        case ControlPanelMessage::WAIT:
            _lcd->print("Waiting for pins");
            break;
    }
    return true;
}

bool ControlPanel::showPinsetterStatus(Pinsetter::PinsetterPose status){
    lastStatus = status;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(4, 2);
    _lcd->print(lastStatus);
    _lcd->print(" ");
    return true;
}

bool ControlPanel::showSlot(uint8_t turretSlot){
    lastLoad =  turretSlot;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(12, 2);
    _lcd->print(lastLoad);
    return true;
}

bool ControlPanel::showMotor(bool motor){
    lastMotor =  motor;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(0, 0);
    if(motor){
        _lcd->print("RUN"); 
    }else{
         _lcd->print("PAUSE");
    }
    return true;
}

bool ControlPanel::showDeck(bool loaded){
    lastDeck = loaded;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(19, 2);
    if(lastDeck){
        _lcd->print("F");
    }else{
        _lcd->print("E");
    }
    return true;
}

void ControlPanel::submitPins(PinSensor::pinStatus pinsAndScore){
    _pindicator.showPinStatus(pinsAndScore.pins);
}

void ControlPanel::clearPins(){
    _pindicator.clear();
}

bool ControlPanel::showSpeed(float speed){
    lastSpeed = speed;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(13, 0);
    if(lastSpeed > 0){
        _lcd->print(lastSpeed);
    }else{
        _lcd->print("-.--");  
    }
    return true;
}

void ControlPanel::notConnected(){
    //this message is important! Connecting/disconnecting while powered can cook the I2C buffer IC. I know because I have done it.
    _lcd->clear();
    _lcd->setCursor(0,0);_lcd->print("Pinsetter not found!");
    _lcd->setCursor(0,2);_lcd->print("DO NOT CONNECT WHILE");
    _lcd->setCursor(0,3);_lcd->print("POWERED!!!");
}