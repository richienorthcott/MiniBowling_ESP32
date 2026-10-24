#pragma once
#include <Player.h>
#include <LiquidCrystal_I2C.h>
#include <Globals.h>

class Game{
    public:
        enum GamePose{
            INIT,
            RUNNING,
            GAMEOVER
        };
    
        GamePose currentPose = GamePose::INIT;

        Game(LiquidCrystal_I2C* lcd);                 
        void init();                                    //initialises a new game for up to 8 players
        void changePlayerCount();
        void changeFrameCount();        
        void printScore();                
        void scroll();                                  //scroll through players on LCD
        Player::ScoreType submitScore(bool isThrowOne, uint8_t score);

        uint8_t numPlayers = 1;                         //this is zero indexed, so 0 means 1
        uint8_t numFrames = 9;                         //this is zero indexed, so 0 means 1
        uint8_t currentFrame = 0;                       //this is zero indexed, so 0 means 1
    
    private:
        Player players[8];                              //allow up to 8 players
        void printPlayer(uint8_t player, bool active);  //prints 20 char summary of player status                                    
        LiquidCrystal_I2C* _lcd;
        uint8_t currentPlayer = 0;                      //this is zero indexed, so 0 means 1        
        uint8_t displayPlayer = 0;                      //this is zero indexed, so 0 means 1        
};