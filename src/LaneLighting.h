#pragma once
#include <Globals.h>
#include <FastLED.h>

class LaneLighting{
    public:
        LaneLighting(CRGB* ledArray);    
        void heartbeat();
        
        void turnOff();
        void turnOn();
        void clear();

        enum LIGHTINGMODE{            
            NORMAL,
            CHASEWHITE,
            CHASEGREEN,
            CHASERAINBOW,
            CHASELAVA,
            STRIKE
        };        

        //static states
        void solidRed();  
        void solidGreen();  
        void solidBlue();
        
        //animations. These all need the heartbeat function to advance.
        void comet();
        void strike();
        void rainbowAnim();
        void lavaAnim();
        void wipeGreen();        
    
    private:                
        CRGB* leds;
        LIGHTINGMODE _currentMode = LIGHTINGMODE::NORMAL;
        uint8_t chaseIndex = 0;        
    };