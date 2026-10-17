# MiniBowling_ESP32

This is an ESP32 based codebase to support Danny Lum's amazing [mini bowling lane](https://dannylum.com/). The file structure is from the Visual Studio Code environment, with [PlatformIO](https://platformio.org) plugin. See `platform.ini` for environment, board and 3rd party libraries used.

## Architecture

The code uses C++ classes to represent the physical layout of the machine through related objects. Timing is critical to this project, therefore the code uses a central timer sending out a regular **heartbeat** notificaiton to the pinsetter, at 30Hz. The pinsetter runs somewhat autonomously, only needing a call to the `doCycle()` function when a ball break is detected. Where there's a risk of sometihng mechanically bad happening, I've tried to include some logic/state checks internally in the Pinsetter code. For example the sweep won't move if the deck is not in the SWEEP or UP state.

Each Pinsetter component has its own ENUM values which tracks its current state, and exposes the public functions needed for its core operation only. Structurally the object hierarchy looks like this:

- main.cpp
    - Pinsetter
        - Deck
            - Slide
            - Scissors
        - Sweep
        - Turret
        - Ball Return Door
        - Deck lighting
        - Throw LEDs

### Public Pinsetter functions

`begin()` call to initialise or reset at any time

`heartbeat()` call at 30Hz from main loop

`turretHomed()` notification from homing sensor interrupt handled in main loop, used on initialisation sequence

`pinSense()` notitifcication from pin IR sensor interrupt handled in main loop

`doCycle(true/false)` cycle the pinsetter. **true** adds a delay before the sweep lowers

 `startMotor()` wakes the motor from a stopped state. This can happen for example if there's a jam in the pinwheel, and the motor sleep timeout is exceeded (default is 30 seconds since last activation from ball throw / pin sense)   

`getStatus()` returns a struct of pinsetter status properties which can be used by the main loop

## Other Components

The main section of the code `main.cpp` handles all the sensor, control panel button and timer interrupts, and acts as a gateway between the pinsetter, and other components, all of which are *optional*, and are handled in their own classes/objects. I2C addresses are configurable in `general_config.h`

**WS2811 Addressable lane LED lights** -  I'm handling this as two strips wired in parallel, so show the same.
**I2C 20x4 LCD display**
**I2C LED matrix display** - for 10 LED 'pindicator' (HT16k33 IC module). Note there's potentially a conflict with the default address since its default (usually 0x70) is the same as the PCA9685's 'All Call' address, which can't be changed. I changed my HT16K33 address to 0x71.
**I2C IO port expander** - for pin sensors (PCF8575 IC module)

The periodic 30Hz **heartbeat** notification is also sent to the LaneLighting instance, so can be used to drive LED animation effects.

Without the fancy control panel, useful functions to attach to physical pushbuttons are:

`pinsetter.doCycle(false)` equivalent to a ball throw, but no delay

`pinsetter.begin()` resets the pinsetter via an initialisation sequence

`pinsetter.startMotor()` restarts the motor, if stopped

The **reset** button doesn't need any complicated coding, its interrupt sets the EN pin HIGH, which disables the stepper driver and the PWM driver (these are both active LOW). It's programmed as a toggle so each subsequent press enters/leaves the reset mode.

## External Hardware

As a minimum with the ESP32 you'll need the PCA9685 PWM IC or module for the servos (there aren't enough pins on an ESP32 to support everything). The default I2C address assumed in the code is 0x40, this can be changed in **general_config.h**. PWM outputs can be connected directly to servos via 220 ohm resistors, other outputs will need interfacing via MOSFETs, optocouplers or similar. Here's the mappings I've used, they can be changed in **pinconfig.h.**

## Default PCA9685 PWM output mappings

All pinsetter functions are from the PWM outputs (up to 16 available on the PCA9685 IC). Conceptually this means as well as the servos, the main motor and all lighting can also be PWM controlled. In practice the servo PWM frequency (50Hz) is too low for LED dimming since the flickering is very noticeable. The code therefore only uses full on/off states. Although the main motor is speed controllable, I've not found a use for that yet!

**Output 0** Deck LHS servo

**Output 1** Sweep RHS servo

**Output 2** Sweep LHS servo

**Output 3** Deck RHS servo

**Output 4** Ball return door servo

**Output 5** Sliding deck servo

**Output 6** Scissors servo

**Output 7** Main motor

**Output 12** Deck RHS white led strip

**Output 13** Deck LHS white led strip

**Output 14** Throw 1 LED

**Output 15** Throw 2 LED

## ESP32 GPIO pins

> All signals need to be level shifted 3.3V <-> 5V. Button pins don't need this, buttons connect the pins to GND

> None of these are *strapping* pins, so no problems uploading code while connected!

**Pin 4** EN to stepper driver and PCA9685 PWM IC (active LOW)

**Pin 13** Pixel data OUT for addressable led strip

**PIN 14** Maintenance LED OUT

**Pin 16** Serial RX (could be integrated with [ScoreMore](https://www.scoremorebowling.com))

**Pin 17** Serial TX (could be integrated with [ScoreMore](https://www.scoremorebowling.com))

**PIN 18** STEP OUT to stepper driver

**PIN 19** DIR OUT to stepper driver

**PIN 21** I2C SDA (data)

**PIN 22** I2C SCL (clock)

**PIN 25** SELECT3 button IN (use 10K pullup resistor to 3.3V)

**PIN 26** MAINTENANCE button IN (use 10K pullup resistor to 3.3V)

**PIN 27** HOME button IN (use 10K pullup resistor to 3.3V)

**PIN 32** SELECT1 button IN (use 10K pullup resistor to 3.3V)

**PIN 33** SELECT2 button IN (use 10K pullup resistor to 3.3V)

**PIN 34** IR pin sensor IN

**PIN 35** Turret hall effect home sensor IN

**PIN 36** Ball speed sensor IN

**PIN 39** Ball break sensor IN
