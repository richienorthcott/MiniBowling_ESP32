#include <Game.h>

Game::Game(LiquidCrystal_I2C* lcd){
    _lcd = lcd;
}

void Game::init(){        
    currentPose = GamePose::RUNNING;
    currentPlayer = 0;
    currentFrame = 0; 
    displayPlayer = 0;
    scrollPlayer = 0;
    for (uint8_t j=0; j < 8; j++) players[j].init();            
}

void Game::printScore(){    
    //print the frame information
    if(currentPose == Game::GamePose::GAMEOVER){
        _lcd->setCursor(8, 0);_lcd->print("OVER           ");
        _lcd->setCursor(7, 3);_lcd->print("      ");         //clear the replay menu optpion            
    }else{
        _lcd->setCursor(15,0);
        if(currentFrame < 9) _lcd->print(0);    //add leading zero if frame <10
       _lcd->print(currentFrame + 1);           //print current frame number
       _lcd->print("/");           
        if(numFrames < 9) _lcd->print(0);       //add leading zero if frame <10
        _lcd->print(numFrames + 1);       //print total frames
    }

    if(numPlayers < 2){
        _lcd->setCursor(0, 3);
        _lcd->print("      ");         //clear the scroll optpion         
    }

    if(currentFrame ==0){
        if(currentPlayer ==0){
            if(players[0].getScore(currentFrame).status == Player::ScoreType::NEW){
                _lcd->setCursor(7, 3);
                _lcd->print("      ");
            }else{
                _lcd->setCursor(7, 3);
                _lcd->print("replay");
            }
        }
    }


    //display format...
    //if one player, use 1st line only
    //if two or more players, active player is top line for plater one, else always bottom line.
    //for first player, scoeboard shows current and next player
    //for all other players, scoreboard shows current and previous player
                    
    if(currentPlayer == 0){        
        if (currentPlayer == displayPlayer) printPlayer(displayPlayer, currentPose != Game::GamePose::GAMEOVER, 1);                                  
        if(numPlayers > 0){
            printPlayer(displayPlayer + 1, false, 2);                                          
        }else{
            _lcd->setCursor(0,2);_lcd->print("                    ");    //clear the 2nd line
        }
    }else{
        printPlayer(displayPlayer-1, false, 1);                                                             
        printPlayer(displayPlayer, currentPose != Game::GamePose::GAMEOVER, 2);                                          
    }
}

void Game::printPlayer(uint8_t player, bool active, uint8_t row){    
    _lcd->setCursor(0, row);
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
            currentPlayer == player && currentFrame == frame ? _lcd->print("_") : _lcd->print("-");                                            
            break;
        case Player::ScoreType::OPEN:
            _lcd->print(players[player].getScore(frame).totalPoints);                
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

bool Game::scroll(){
    if(numPlayers < 2 )return false; //no need to scroll as there's one player;
    printPlayer(scrollPlayer, currentPlayer == scrollPlayer && currentPose != Game::GamePose::GAMEOVER, 1);
    printPlayer(scrollPlayer + 1, currentPlayer == scrollPlayer + 1 && currentPose != Game::GamePose::GAMEOVER, 2);
    scrollPlayer += 1;
    if(scrollPlayer >= numPlayers) scrollPlayer = 0;    
    return true;
}

Player::ScoreType Game::submitScore(bool isThrowOne, uint8_t score){
    
    Player::ScoreType ret = Player::ScoreType::OPEN;

    if(currentPose == Game::GamePose::RUNNING){
        //submit a score to the game        
        ret = submitToGame(isThrowOne, score);
    }else{
        //submit to get back frame status, but don't persist it.        
        if(isThrowOne){
            lastThrowOne = score;
            if (score == 10) ret = Player::ScoreType::STRIKE;
        }else{                        
            if(score + lastThrowOne == 10) ret = Player::ScoreType::SPARE;        
            lastThrowOne = 0;
        }        
    }
    return ret;                
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
                if(currentFrame > numFrames) currentPose = Game::GamePose::GAMEOVER;              
            }
        }
    }    
    return ret;
}

bool Game::replayFrame(){    
    if (currentPose != Game::GamePose::RUNNING) return false;

    //remove the current frame, if in progress
    if (players[currentPlayer].getScore(currentFrame).status != Player::ScoreType::NEW){
        players[currentPlayer].resetFrame(currentFrame);
        printScore();
        return true;
    }
    
    //remove the last submitted frame
    if(currentPlayer > 0){
        currentPlayer -= 1;
        displayPlayer = currentPlayer;
        players[currentPlayer].resetFrame(currentFrame);
        printScore();
        return true;
    }else{
        if (currentFrame>0){
            currentFrame -= 1;
            currentPlayer = numPlayers;
            displayPlayer = currentPlayer;
            players[currentPlayer].resetFrame(currentFrame);
            printScore();
            return true;
        }   
    }    
    return false;//nothing to clear
}