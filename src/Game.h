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
        
        void changePlayerCount();                       //increments the palyer count
        void changePlayerCount(uint8_t totalPlayers);   //set the number of players
        
        void changeFrameCount();                        //increments the frame count
        void changeFrameCount(uint8_t totalFrames);     //set the number of frames
                
        void printScore();                              //update lastest score to LCD
        bool scroll();                                  //scroll through players on LCD if more than 2
        bool replayFrame();                             //deletes the last played frame

        Player::ScoreType submitScore(bool isThrowOne, uint8_t score);        

        uint8_t numPlayers = 0;                         //this is zero indexed, so 0 means 1
        uint8_t numFrames = 9;                          //this is zero indexed, so 0 means 1                
    private:
        Player players[8];                                              //allow up to 8 players
        void printPlayer(uint8_t player, bool active, uint8_t row);     //prints 20 char summary of player status                                    
        void printFrame(uint8_t player, uint8_t frame);                 //used by printPlayer function. Prints a frame outcome
        LiquidCrystal_I2C* _lcd;
        Player::ScoreType submitToGame(bool isThrowOne, uint8_t score);           
                          
        uint8_t currentPlayer = 0;                      //this is zero indexed, so 0 means 1
        uint8_t currentFrame = 0;                       //this is zero indexed, so 0 means 1
        uint8_t displayPlayer = 0;             
        uint8_t lastThrowOne = 0;     
        uint8_t scrollPlayer = 0;           
};