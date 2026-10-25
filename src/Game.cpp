#include <Game.h>

Game::Game(){}

Game::Game(LiquidCrystal_I2C* lcd){
    _lcd = lcd;
}

void Game::init(){        
    currentPose = GamePose::RUNNING;
    currentPlayer = 0;
    currentFrame = 0; 
    displayPlayer = 0;
    for (uint8_t j=0; j < 8; j++) players[j].init();            
}

void Game::printScore(){    
    _lcd->setCursor(15,0);
        
    if(currentPose == Game::GamePose::GAMEOVER){
        _lcd->print("OVER ");            
    }else{
        if(currentFrame < 9) _lcd->print(0);    //write leading zero to frame counter        
       _lcd->print(currentFrame + 1);    
       _lcd->print("/");           
        if(numFrames < 9) _lcd->print(0);    //write leading zero to frame counter
        _lcd->print(this->numFrames + 1);                
    }
    
    _lcd->setCursor(0, 1);        
    if(currentPlayer==0){
        Serial.print(">");    
        Serial.println(displayPlayer);    
        if(numPlayers>0) Serial.println(" 1");
    }else{
        Serial.print(" ");
        Serial.println(displayPlayer-1);
        Serial.print(">");
        Serial.println(displayPlayer);
    }
    
    
    
    this->printPlayer(displayPlayer, displayPlayer == currentPlayer && currentPose != Game::GamePose::GAMEOVER, 1);                          
    
    _lcd->setCursor(0, 2);            
    if(displayPlayer < numPlayers){
        this->printPlayer(displayPlayer + 1, (displayPlayer +1) == currentPlayer, 2);                          
    }else{
        _lcd->print("                    ");        
    }
}

void Game::debugScore(){
    //_lcd->setCursor(0, 1);        
    //this->printPlayer(displayPlayer, displayPlayer == currentPlayer && currentPose != Game::GamePose::GAMEOVER, 1);                          

    
    if(currentPlayer==0){
        if (currentPlayer == displayPlayer) Serial.print(">");    
        Serial.println(displayPlayer);    
        if(numPlayers > 0){
            Serial.print(" ");
            Serial.println(displayPlayer + 1);
        }else{
            Serial.println();
        }
    }else{
        Serial.print(" ");
        Serial.println(displayPlayer- 1);
        if (currentPlayer == displayPlayer) Serial.print(">");    
        Serial.println(displayPlayer);
    }
}



void Game::printPlayer(uint8_t player, bool active, uint8_t row){    
    if(active){
        _lcd->print(">");    
    }else{
        _lcd->print(" ");        
    }
    _lcd->print(player + 1);    
    _lcd->print(":");
        
    //print frames 1-last frame
    for(uint8_t i = 0; i <= numFrames; i++) printFrame(player, i);
    _lcd->print("  ");//clear the bonus frame numbers, important if changing displayed line
    _lcd->setCursor(4 + numFrames, row);    //optionally print the two extra frames if used
    if (players[player].getScore(numFrames + 2).status != Player::ScoreType::NEW) printFrame(player, numFrames + 2);
    if (players[player].getScore(numFrames + 1).status != Player::ScoreType::NEW) printFrame(player, numFrames + 1);
    
    //write the total
    _lcd->setCursor(15, row); _lcd->print("t:");
    int playerTotal = players[player].getTotal(numFrames);
    if(playerTotal < 100) _lcd->print("0");
    if(playerTotal < 10) _lcd->print("0");
    _lcd->print(playerTotal);    
}

void Game::printFrame(uint8_t player, uint8_t frame){    
    //prints a single character representing the frame outcome
    switch(players[player].getScore(frame).status){
         case Player::ScoreType::NEW:
            if(currentPlayer == player && currentFrame == frame){
                _lcd->print("_");                    
            }else{
                _lcd->print("-");                    
            }
            break;
        case Player::ScoreType::OPEN:
            _lcd->print(players[player].getScore(frame).score);                
            break;
        case Player::ScoreType::SPARE:
            _lcd->print("/");                
            break;           
        case Player::ScoreType::STRIKE:
            _lcd->print("X");                
            break;            
    }        
}

void Game::changePlayerCount(uint8_t totalPlayers){
    if (totalPlayers > 0) numPlayers = totalPlayers-1;
}

void Game::changeFrameCount(uint8_t totalFrames){
    if (totalFrames > 0) numFrames = totalFrames-1;  
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
    if (displayPlayer >= numPlayers) displayPlayer = 0;
    printScore();
}

Player::ScoreType Game::submitScore(bool isThrowOne, uint8_t score){
    if(currentPose == Game::GamePose::RUNNING){
        //submit a score to the game        
        return this->submitToGame(isThrowOne, score);
    }else{
        //submit to get back frame status, but don't persist it.
        Player::ScoreType ret = Player::ScoreType::OPEN;
        if(isThrowOne){
            lastThrowOne = score;
            if (score == 10) ret = Player::ScoreType::STRIKE;
        }else{                        
            if(score + lastThrowOne == 10) ret = Player::ScoreType::SPARE;        
            lastThrowOne = 0;
        }        
        return ret;                
    }
}

Player::ScoreType Game::submitToGame(bool isThrowOne, uint8_t score){

    Player::ScoreType ret;
    displayPlayer = currentPlayer;    

    //this section handles the possible extra throws on the last frame
    if(players[currentPlayer].extraThrows == 1){        
        players[currentPlayer].setScore(currentFrame + 1, true, score);        
        players[currentPlayer].setScore(currentFrame + 1, false, 0);                    
        players[currentPlayer].extraThrows = 0;
        ret = Player::ScoreType::EXTRAOPEN;                
    
    }else if(players[currentPlayer].extraThrows == 2){
        //return type will be OPEN or STRIKE
        ret = players[currentPlayer].setScore(currentFrame +2 , true, score);        
        players[currentPlayer].setScore(currentFrame +2 , false, 0);                    
        players[currentPlayer].extraThrows = 1;
    
    }else{
        ret = players[currentPlayer].setScore(currentFrame, isThrowOne, score);            
        
        if(currentFrame == numFrames && ret == Player::ScoreType::STRIKE) players[currentPlayer].extraThrows = 2;
        if(currentFrame == numFrames && ret == Player::ScoreType::SPARE) players[currentPlayer].extraThrows = 1;            
    }

    if (players[currentPlayer].extraThrows == 0){
        if(!isThrowOne || ret == Player::ScoreType::STRIKE || ret == Player::ScoreType::EXTRAOPEN){
            currentPlayer += 1;
            displayPlayer += 1;
            
            if(currentPlayer > numPlayers){                
                currentFrame += 1;            
                currentPlayer = 0;                        
                displayPlayer = 0;
                if(currentFrame > numFrames) this->currentPose = Game::GamePose::GAMEOVER;              
            }
        }
    }    
    return ret;
}