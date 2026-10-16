#include <LaneLighting.h>

LaneLighting::LaneLighting(CRGB* ledArray) {
    leds = ledArray;    
    FastLED.addLeds<WS2812B, LANE_PIXELS_PIN, GRB>(leds, LANE_PIXEL_COUNT);
    FastLED.setBrightness(LANE_PIXELS_INTENSITY1);
}

void LaneLighting::turnOff(){
    FastLED.setBrightness(0);
    FastLED.show();
}

void LaneLighting::turnOn(){
    FastLED.setBrightness(LANE_PIXELS_INTENSITY1);
    FastLED.show();
}

void LaneLighting::solidRed(){
    _currentMode = LIGHTINGMODE::NORMAL;
    fill_solid(leds, LANE_PIXEL_COUNT, CRGB::Red);
    FastLED.show();
}

void LaneLighting::solidGreen(){
    _currentMode = LIGHTINGMODE::NORMAL;
    fill_solid(leds, LANE_PIXEL_COUNT, CRGB::Green);
    FastLED.show();
}

void LaneLighting::solidBlue(){
    _currentMode = LIGHTINGMODE::NORMAL;
    fill_solid(leds, LANE_PIXEL_COUNT, CRGB::Blue);
    FastLED.show();
}

void LaneLighting::comet(){
    chaseIndex = LANE_PIXEL_COUNT;
    _currentMode = LIGHTINGMODE::CHASEWHITE;
    FastLED.setBrightness(LANE_PIXELS_INTENSITY2);
}

void LaneLighting::wipeGreen(){
    chaseIndex = 0;
    FastLED.setBrightness(LANE_PIXELS_INTENSITY1);
    _currentMode = LIGHTINGMODE::CHASEGREEN;
}

void LaneLighting::strike(){
    chaseIndex = 0;
    fill_solid(leds, LANE_PIXEL_COUNT, CRGB::Black);
    FastLED.show();
    FastLED.setBrightness(LANE_PIXELS_INTENSITY2);
    _currentMode = LIGHTINGMODE::STRIKE;
}

void LaneLighting::rainbowAnim(){
    chaseIndex = 0;    
    FastLED.setBrightness(LANE_PIXELS_INTENSITY1);
    _currentMode = LIGHTINGMODE::CHASERAINBOW;
}


void LaneLighting::heartbeat(){
    #define step 5

    switch(_currentMode){
        case LIGHTINGMODE::CHASEWHITE:
            fill_solid(leds, LANE_PIXEL_COUNT, CRGB::Black);
            leds[chaseIndex] = CRGB::White;
            leds[chaseIndex - 1] = CRGB::White;
            leds[chaseIndex - 2] = CRGB::White;
            leds[chaseIndex - 3] = CRGB::White;
            leds[chaseIndex - 4] = CRGB::White;
            chaseIndex -= step;
            FastLED.show();

            if(chaseIndex < step ){
                FastLED.setBrightness(LANE_PIXELS_INTENSITY1);
                this->solidRed();
            } 
            break;
        case LIGHTINGMODE::CHASERAINBOW:
            fill_palette(leds, LANE_PIXEL_COUNT, chaseIndex, 3, RainbowColors_p, 255, fl::LINEARBLEND);            
            chaseIndex += 1;
            FastLED.show();
            break;        
        case LIGHTINGMODE::CHASEGREEN:
            leds[chaseIndex] = CRGB::Green;
            leds[chaseIndex +1 ] = CRGB::Green;
            FastLED.show();
            chaseIndex += 2;
            if(chaseIndex >= LANE_PIXEL_COUNT) this -> solidGreen();
            break;
        case LIGHTINGMODE::STRIKE:
            if(chaseIndex ==0){
                for(uint8_t i=0;i<LANE_PIXEL_COUNT;i++){
                    if (random8() < 50){
                        leds[i] = CRGB::White;
                    }else{
                        leds[i] = CRGB::Black;
                    }
                }  
                FastLED.show();
            }
            chaseIndex += 1 ;
            //show the effect every 2 frames
            if(chaseIndex==2)chaseIndex=0;
            break;
        }
}

void LaneLighting::clear(){
    _currentMode = LIGHTINGMODE::NORMAL;
    FastLED.clear();
    FastLED.show();
}