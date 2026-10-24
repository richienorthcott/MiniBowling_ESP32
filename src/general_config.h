#pragma once

//#define DEBUG

#ifdef DEBUG
  #define DEBUG_LOG(source, action) Serial.print(millis());Serial.print(" - ");Serial.print(source);Serial.print(":");Serial.println(action)
  #else
  #define DEBUG_LOG(source, action)
#endif

#define VERSION 1.23

#define SERIAL_BAUD 115200
#define TIMER_FREQEUCNCY 33333 // 1,000,000 /33333 = 30Hz

//I2C addresses
#define LCD_ADDRESS         0x3F
#define PINDICATOR_ADDRESS  0x71
#define PCA9685_ADDRESS     0x40
#define PCF8575_ADDRESS     0x20

#define LCD_ROWS 4
#define LCD_COLS 20

#define SERVO_PWM_FREQUENCY 50 //HZ
//servo positions (pulse widths/4095)
#define BALL_DOOR_OPEN    470
#define BALL_DOOR_CLOSE   140
#define SLIDE_RECEIVE     400
#define SLIDE_DUMP        185  
#define SCISSORS_OPEN     255
#define SCISSORS_CLOSE    350
#define SWEEP_LEFT_PULSEWIDTH_UP      315
#define SWEEP_LEFT_PULSEWIDTH_GUARD   258 
#define SWEEP_LEFT_PULSEWIDTH_BACK    188
#define SWEEP_RIGHT_PULSEWIDTH_UP     171
#define SWEEP_RIGHT_PULSEWIDTH_GUARD  231 
#define SWEEP_RIGHT_PULSEWIDTH_BACK   307
#define DECK_LEFT_PULSEWIDTH_UP 452
#define DECK_LEFT_PULSEWIDTH_SWEEP 357
#define DECK_LEFT_PULSEWIDTH_GRAB 208//218
#define DECK_LEFT_PULSEWIDTH_DOWN 103//93
#define DECK_RIGHT_PULSEWIDTH_UP  162
#define DECK_RIGHT_PULSEWIDTH_SWEEP 256
#define DECK_RIGHT_PULSEWIDTH_GRAB 403//393
#define DECK_RIGHT_PULSEWIDTH_DOWN 507//517

//ball return door timing (in frames)
#define BALL_DOOR_DELAY_PULSES        200 //delay from ball trigger to door opening
#define BALL_DOOR_CLOSE_PULSES        360 //delay from ball trigger to door closing

#define HOME_DEBOUNCE_MILLIS 1000

#define TURRET_STEPS_PER_REV 1600
#define TURRET_HOME_OFFSET 77
#define TURRET_POSITION1  133 
#define TURRET_POSITION2  267
#define TURRET_POSITION3  533
#define TURRET_POSITION4  667
#define TURRET_POSITION5  800
#define TURRET_POSITION6  1066
#define TURRET_POSITION7  1200
#define TURRET_POSITION8  1333
#define TURRET_POSITION9  1466

#define TURRET_SPEED_HOMING   100
#define TURRET_ACCEL_HOMING   400
#define TURRET_SPEED_RUN      1500
#define TURRET_ACCEL_RUN      2000
#define TURRET_MOVE_OFF_SENSOR  80
#define TURRET_ADVANCE_DELAY_MS 800  //milliseconds
#define TURRET_PAUSE_SLOT  8 //turret will pause conveyor on this slot if the deck has pins in, to prevent jams 
#define TURRET_CCW -1 //set to 1 or -1 for CW or CCW. Or change the motor connections over! 
#define TURRET_CW 1 //set to 1 or -1 for CW or CCW. Or change the motor connections over! 

#define LANE_PIXEL_COUNT        86
#define LANE_PIXELS_INTENSITY1  80
#define LANE_PIXELS_INTENSITY2  255
#define PINDICATOR_INTENSITY    5 //0-16

#define MAX_PWM 4095

#define DEBOUNCE_MS 300 //milliseconds
#define SENSOR_DEBOUNCE_MS 600 //milliseconds

#define CONVEYOR_TIMEOUT_MS 30000 //milliseconds
#define MAX_BALL_SPEED 800     //ignore any speed sensings bigger than this

//pinsetter motion timings (in 'frames')
#define TWEEN_FRAMES 20
#define TURRET_FULL_ROTATE 80
#define DECK_UP_TO_GRAB 30
#define DECK_SWEEP_TO_GRAB 20
#define DECK_SCISSORS_OPEN_CLOSE 20
#define DECK_UP_SWEEP 20
#define DECK_DOWN_TO_UP 40
#define THROW_DELAY 40 //between ball sense and sweep activation
#define PIN_RELEASE 25
#define DECK_MID_RISE 20
#define DECK_UP 30
#define CYCLE_DELAY 40
