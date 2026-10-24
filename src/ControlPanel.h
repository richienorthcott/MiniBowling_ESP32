#pragma once
#include <LiquidCrystal_I2C.h>
/*
#include <ht16k33.h>
#include <Pindicator.h>
#include <PinSensor.h>
*/
#include <Pinsetter.h>
#include <Game.h>
#include <Globals.h>

class ControlPanel{
    public:
        ControlPanel(LiquidCrystal_I2C* lcd);
        
        enum ControlPanelMessage{
            INITIALISING,
            WAIT,
            CYCLING,
            BOWL1,
            BOWL2
        };
        
        enum ControlPanelPage{
            INIT,
            MAIN,
            UTIL,
            LIGHT,            
            GAME,
            SETUP
        };

        ControlPanelPage currentPose = ControlPanelPage::INIT;
        void begin();

        bool showMessage(ControlPanel::ControlPanelMessage msg);      //prints a preset message according to the passed ENUM. Returns false if not in MAIN mode
        bool showSpeed(float speed);                                  //prints the last ball throw speed. Returns false if not in MAIN mode
        bool showPinsetterStatus(Pinsetter::PinsetterPose status);    //prints the number of the active pinsetter state enum. Returns false if not in MAIN mode
        bool showSlot(uint8_t turretSlot);                            //prints the number of the active turret load slot. Returns false if not in MAIN mode
        bool showDeck(bool loaded);                                   //prints whether the deck is full or empty. Returns false if not in MAIN mode
        bool showMotor(bool motor);                                   //prints the state of the main motor. Returns false if not in MAIN mode

        Player::ScoreType submitPins(uint8_t score, bool isThrowOne);                

        //these are all different overall menu 'modes'
        void notConnected();
        void mainMenu();
        void gameMenu();
        void setupMenu();
        void utilMenu();
        void lightMenu();

        bool updatePlayerCount();
        bool updateFrameCount();
        bool initGame();
        bool scrollGame();

    private:        
        void initMenu();
        LiquidCrystal_I2C* _lcd;    
        //these variables persist the menu status if changing page
        float lastSpeed = 0;
        uint8_t lastLoad = 0;
        ControlPanelMessage lastMessage = ControlPanelMessage::WAIT;
        Pinsetter::PinsetterPose lastStatus = Pinsetter::PinsetterPose::INIT;
        bool lastDeck = false;
        bool lastMotor = false;
        Game _game;
        
};
