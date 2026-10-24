#include <Game.h>

Game::Game(LiquidCrystal_I2C* lcd){
    _lcd = lcd;
}

void Game::init(){        
    currentPose = GamePose::RUNNING;
    currentPlayer = 0;
    currentFrame = 0; 
    displayPlayer = 0;

    for(uint8_t j=0; j < 8; j++){
        players[j].init();            
    }
}

void Game::printScore(){
    
    Serial.print("score:");
    Serial.print(currentPlayer);
    Serial.print(",");
    Serial.print(displayPlayer);
    Serial.println();
    

    //update the frame counter
    _lcd->setCursor(15,0);
    
    if(currentPose == Game::GamePose::GAMEOVER){
        _lcd->print("DONE ");    
    }else{
        if(currentFrame < 9) _lcd->print(0);    //write leading zero to frame counter
        _lcd->print(this->currentFrame + 1);    
        _lcd->print("/");       
    
        if(numFrames < 9) Serial.print(0);    //write leading zero to frame counter
        _lcd->print(this->numFrames + 1);    
    }
    
    _lcd->setCursor(0, 1);this->printPlayer(displayPlayer, displayPlayer == currentPlayer);                      
    _lcd->setCursor(0, 2);        
    if(displayPlayer < numPlayers){
        this->printPlayer(displayPlayer + 1, (displayPlayer +1) == currentPlayer);                          
    }else{
        _lcd->print("                    ");
    }
}

void Game::printPlayer(uint8_t player, bool active){    
    if(active){
        _lcd->print(">");
    }else{
        _lcd->print(" ");
    }
    _lcd->print(player + 1);
    _lcd->print(":");
    for(uint8_t i = 0; i < 10; i++){
        switch(players[player].getScore(i).status){
            case Player::ScoreType::NEW:
                if(currentPlayer == player && currentFrame ==i){
                    _lcd->print("_");
                }else{
                    _lcd->print("-");
                }
                break;
            case Player::ScoreType::OPEN:
                _lcd->print(players[player].getScore(i).score);
                break;
            case Player::ScoreType::SPARE:
                _lcd->print("/");
                break;            
            case Player::ScoreType::STRIKE:
                _lcd->print("X");
                break;            
        }        
    }
    
    //write the total
    _lcd->print("  t:");
    int playerTotal = players[player].getTotal();
    if(playerTotal < 100) _lcd->print("0");
    if(playerTotal < 10) _lcd->print("0");
    _lcd->print(playerTotal);    
}

void Game::changePlayerCount(){
    currentPose = GamePose::INIT;
    numPlayers += 1;
    if (numPlayers == 8) numPlayers = 0;
    _lcd->setCursor(14, 1);
    _lcd->print(numPlayers + 1); 
}

void Game::changeFrameCount(){
    currentPose = GamePose::INIT;
    numFrames += 1;
    if (numFrames == 10) numFrames = 0;
    _lcd->setCursor(14, 2);
    _lcd->print(numFrames + 1);
    _lcd->print(" ");    
} 

void Game::scroll(){
    displayPlayer += 1;
    if (displayPlayer == numPlayers) displayPlayer = 0;
    printScore();
}

Player::ScoreType Game::submitScore(bool isThrowOne, uint8_t score){
    displayPlayer = currentPlayer;    
    
    Player::ScoreType ret = players[currentPlayer].setScore(currentFrame, isThrowOne, score);
    
    if(!isThrowOne){
        currentPlayer += 1;
        if(currentPlayer > numPlayers){
            currentFrame += 1;
            if(currentFrame == numFrames) this->currentPose = Game::GamePose::GAMEOVER;
            currentPlayer = 0;
            displayPlayer = 0;
        }
    }
    return ret;
}