#pragma once
#include <Arduino.h>

class Player{
    public:
        Player();

        enum ScoreType{
            NEW,            //frame has not been played        
            OPEN,           //frame has points, but not spare or strike. Active after submitting throw1 (unless strike)
            SPARE,          //frame was a spare (all pins knocked down on 1st and 2nd throw, or 2nd throw only)
            STRIKE          //frame was a strike (all pins knocked down on throw 1)
        };
        
        struct FrameScore{  //struct used for scoreboard
            uint8_t score;
            ScoreType status;
        };
        void init();        //reset the player for a new game
        bool setScore(uint8_t frame, bool throw1, uint8_t score);   //set points by frame and throw1/throw2 status 
        FrameScore getScore(uint8_t frame);                         //returns the total points for a frame, and its status.
        int getTotal();                                             //returns the total points for this player
    private:        
        uint8_t _throw[2][12] = {};
        ScoreType _type[12];
};