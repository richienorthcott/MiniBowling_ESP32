#include <Player.h>

Player::Player(){            
    this->init();
}

void Player::init(){
    //reset all frame scores for player
    for(uint8_t i=0; i<12; i++){            
        this->setScore(i, true, 0);
        this->setScore(i, false, 0);            
        _type[i] = Player::ScoreType::NEW;   
    }        
}

Player::ScoreType Player::setScore(uint8_t frame, bool throw1, uint8_t score){

    
    if(throw1){
        _throw[0][frame] = score;
        if(score == 10){
            _type[frame] = ScoreType::STRIKE;
        }else{
            _type[frame] = ScoreType::OPEN;
        }
    }else{
        //this is throw 2
        if(_throw[0][frame] + score > 10){
            _throw[1][frame] = 10 - _throw[0][frame];        
        }else{
            _throw[1][frame] = score;
        }        
        if(score + _throw[0][frame] == 10) _type[frame] = ScoreType::SPARE;        
    }
    return _type[frame];
}

Player::FrameScore Player::getScore(uint8_t frame){
    Player::FrameScore ret;
    ret.score = _throw[0][frame] + _throw[1][frame];
    ret.status = _type[frame];
    return ret;
}

int Player::getTotal(){
    int retTotal = 0;
    
    for(uint8_t i=0;i<10;i++){
        retTotal += _throw[0][i] + _throw[1][i];
        switch(_type[i]){
            case ScoreType::SPARE:
                retTotal += _throw[0][i+1];
                break;
            case ScoreType::STRIKE:
                if(_throw[0][i+1] ==10){
                    retTotal += 10;
                    retTotal += _throw[0][i+2];    
                }else{
                    retTotal += _throw[0][i+1];
                    retTotal += _throw[1][i+1];
                }
                break;
            default:
                break;
        }
    }
    return retTotal;
}