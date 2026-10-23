#include <ControlPanel.h>

ControlPanel::ControlPanel(LiquidCrystal_I2C* lcd, HT16K33* ledMatrix):_pindicator(ledMatrix),_game(lcd){
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
    _lcd->setCursor(0, 0); _lcd->print("    MINI BOWLING    ");
    _lcd->setCursor(0, 1); _lcd->print("    ============    ");
    _lcd->setCursor(0, 3); _lcd->print("               v1.23");
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
    _lcd->setCursor(0, 3);_lcd->print("cycle | game | util ");   
}

void ControlPanel::utilMenu(){
    currentPose = ControlPanelPage::UTIL;
    _lcd->clear();
    _lcd->setCursor(0, 0);_lcd->print("PINSETTER UTILITIES");
    _lcd->setCursor(0, 3);_lcd->print("light |motor | reset");
}

void ControlPanel::gameMenu(){
    currentPose = ControlPanelPage::GAME;
    _lcd->clear();        
    _lcd->setCursor(0, 0);
    
    _lcd->print(_game.numPlayers + 1);
    _lcd->print("P GAME  frame:--/--");    
    switch(_game.currentPose){
        case Game::GamePose::INIT:            
            _lcd->setCursor(0, 1); _lcd->print(" Please set up game ");            
            _lcd->setCursor(0, 2); _lcd->print("                    ");                    
             _lcd->setCursor(0, 3);_lcd->print("      |      | setup");
   
            break;
        default:            
            _lcd->setCursor(0, 3);_lcd->print("scroll|rebowl| setup");    
            _game.printScore();            
            break;
        }
} 

void ControlPanel::setupMenu(){
    currentPose = ControlPanelPage::SETUP;
    _lcd->clear();    
    _lcd->setCursor(0, 0);_lcd->print("GAME SETUP");
    
    _lcd->setCursor(0, 1);
    _lcd->print("     #Players:");
    _lcd->print(_game.numPlayers + 1);
    
    _lcd->setCursor(0, 2);
    _lcd->print("      #Frames:");
    _lcd->print(_game.numFrames + 1);
    _lcd->print(" ");
    
    _lcd->setCursor(0, 3);_lcd->print("player|frames| begin");
}

void ControlPanel::lightMenu(){
    currentPose = ControlPanelPage::LIGHT;
    _lcd->clear();
    _lcd->setCursor(0, 0);_lcd->print("LIGHT");
    _lcd->setCursor(0, 3);_lcd->print("rainbo|strike|wipe  ");
}

bool ControlPanel::showMessage(ControlPanel::ControlPanelMessage msg){
    lastMessage = msg;
    if(currentPose != ControlPanelPage::MAIN) return false;
    _lcd->setCursor(0, 1);
    _lcd->print("msg:");
    switch(lastMessage){
        case ControlPanelMessage::INITIALISING:    
            _lcd->print("Initialising    ");
            break;
        case ControlPanelMessage::BOWL1:
            _lcd->print("Bowl 1st ball   ");
            break;
        case ControlPanelMessage::BOWL2:
            _lcd->print("Bowl 2nd ball   ");
            break;
        case ControlPanelMessage::CYCLING:
            _lcd->print("Cycling         ");
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
        _lcd->print("RUN  "); 
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

bool ControlPanel::updatePlayerCount(){
    if(currentPose != ControlPanelPage::SETUP) return false;
    _game.changePlayerCount();
    return true;
}

bool ControlPanel::updateFrameCount(){
    if(currentPose != ControlPanelPage::SETUP) return  false;
    _game.changeFrameCount();
    return true;
}

bool ControlPanel::initGame(){
    if(currentPose != ControlPanelPage::SETUP) return false;
    _game.init();
    this->gameMenu();
    return true;
}

bool ControlPanel::scrollGame(){
    if(currentPose != ControlPanelPage::GAME) return false;
    if(_game.currentPose != Game::GamePose::RUNNING) return false;
    _game.scroll();
    return true;
}