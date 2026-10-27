#pragma once
#include <LiquidCrystal_I2C.h>
#include <Pinsetter.h>
#include <Game.h>
#include <Globals.h>

class ControlPanel{
    public:        
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
        
        ControlPanel(LiquidCrystal_I2C* lcd);
        ControlPanelPage currentPose = ControlPanelPage::INIT;
        void begin();

        bool showMessage(ControlPanel::ControlPanelMessage msg);      //prints a preset message according to the passed ENUM. Returns false if not in MAIN mode
        bool showSpeed(float speed);                                  //prints the last ball throw speed. Returns false if not in MAIN mode
        bool showPinsetterStatus(Pinsetter::PinsetterPose status);    //prints the number of the active pinsetter state enum. Returns false if not in MAIN mode
        bool showSlot(uint8_t turretSlot);                            //prints the number of the active turret load slot. Returns false if not in MAIN mode
        bool showDeck(bool loaded);                                   //prints whether the deck is full or empty. Returns false if not in MAIN mode
        bool showMotor(bool motor);                                   //prints the state of the main motor. Returns false if not in MAIN mode
        
        //these initiate the different LCD menu modes
        void notConnected();    
        void mainMenu();       
        void gameMenu();       
        void setupMenu();      
        void utilMenu();       
        void lightMenu();      

        //these functions are relayed to the Game object. By having this as a child object of the control panel, we can easily check if we're on the right display mode.
        bool updatePlayerCount();       //relay to Game object. Retrurns false if not in game setup mode.
        bool updateFrameCount();        //relay to Game object. Retrurns false if not in game setup mode.
        bool initGame();                //relay to Game object. Retrurns false if not in game setup mode.
        bool scrollGame();              //relay to Game object. Retrurns false if not in game mode.
        bool replayFrame();              //relay to Game object. Retrurns false if not in game mode.
        Player::ScoreType submitPins(uint8_t score, bool isThrowOne);                
    private:        
        LiquidCrystal_I2C* _lcd;                                                
        Game _game;

        void initMenu();                                                        //the init mode can't be called by other classes

        float lastSpeed = 0;                                                    //persist value when not displayed
        uint8_t lastLoad = 0;                                                   //persist value when not displayed
        ControlPanelMessage lastMessage = ControlPanelMessage::INITIALISING;    //persist value when not displayed
        Pinsetter::PinsetterPose lastStatus = Pinsetter::PinsetterPose::INIT;   //persist value when not displayed
        bool lastDeck = false;                                                  //persist value when not displayed
        bool lastMotor = false;                                                 //persist value when not displayed        
};
