#include <Globals.h>
#include <Game.h>

Game myGame;


void setup(){        
    Serial.begin(115200);
    while(!Serial);
    
    myGame.changePlayerCount(3);
    myGame.changeFrameCount(1);    
    myGame.init();            

    Serial.println();

    myGame.debugScore();    
}

void loop(){           
        myGame.submitScore(true, 1);
        if(myGame.currentPose == Game::GamePose::RUNNING) myGame.debugScore();
        myGame.submitScore(false, 0);
        if(myGame.currentPose == Game::GamePose::RUNNING) myGame.debugScore();
}