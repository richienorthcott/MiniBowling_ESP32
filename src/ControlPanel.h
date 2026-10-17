#pragma once
#include <LiquidCrystal_I2C.h>
#include <ht16k33.h>
#include <Globals.h>
#include <Pindicator.h>
#include <PinSensor.h>
#include <Pinsetter.h>

class ControlPanel{
    public:
        ControlPanel(LiquidCrystal_I2C* lcd, HT16K33* ledMatrix);
        
        enum ControlPanelMessage{
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
            SCORE,
            GAME
        };

        ControlPanelPage currentPose = ControlPanelPage::INIT;
        void begin();
        void notConnected();
        
        bool showMessage(ControlPanel::ControlPanelMessage msg);
        bool showSpeed(float speed);
        bool showPinsetterStatus(Pinsetter::PinsetterPose status);//prints the number of the active pinsetter state enum
        bool showSlot(uint8_t turretSlot);
        bool showDeck(bool loaded);
        bool showMotor(bool motor);

        void submitPins(PinSensor::pinStatus pinsAndScore);
        void clearPins();
        
        void mainMenu();//root level
        void utilMenu();//level 1
        void lightMenu();//level 2
        void scoreMenu();//level 2
    private:        
        void initMenu();
        LiquidCrystal_I2C* _lcd;
        HT16K33* _ledMatrix;
        Pindicator _pindicator;

        //these four variables persist the menu status if changing page
        float lastSpeed = 0;
        uint8_t lastLoad = 0;
        ControlPanelMessage lastMessage = ControlPanelMessage::WAIT;
        Pinsetter::PinsetterPose lastStatus = Pinsetter::PinsetterPose::INIT;
        bool lastDeck = false;
        bool lastMotor = false;
};