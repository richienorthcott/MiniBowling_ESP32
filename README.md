# MiniBowling_ESP32

This is as per the main branch but without display, menu system and speed sensor (as there's nothing to display the speed on!). The reset button is still available, select buttons 1-3 have the folllowing fixed functions:

**Select 1** - cycle (no delay)

**Select 2** - start motor (if stopped)

**Select 3** - reset everything to power on state

## ESP32 GPIO pins

> All signals need to be level shifted 3.3V <-> 5V. Button pins don't need this, buttons connect the pins to GND

> None of these are *strapping* pins, so no problems uploading code while connected!

**GPIO 4** EN to stepper driver and PCA9685 PWM IC (active LOW)

**GPIO 13** Pixel data OUT for addressable led strip

**GPIO 14** Maintenance LED OUT

**GPIO 16** Serial RX. Not in use (could be integrated with [ScoreMore](https://www.scoremorebowling.com))

**GPIO 17** Serial TX. Not in use (could be integrated with [ScoreMore](https://www.scoremorebowling.com))

**GPIO 18** STEP OUT to any suitable stepper driver. I'm using a TMC2009 with the smaller NEMA17 motor.

**GPIO 19** DIR OUT to any suitable stepper driver. I'm using a TMC2009 with the smaller NEMA17 motor.

**GPIO 21** I2C SDA (data)

**GPIO 22** I2C SCL (clock)

**GPIO 25** SELECT3 button IN (use 10K pullup resistor to 3.3V)

**GPIO 26** MAINTENANCE button IN (use 10K pullup resistor to 3.3V)

**GPIO 32** SELECT1 button IN (use 10K pullup resistor to 3.3V)

**GPIO 33** SELECT2 button IN (use 10K pullup resistor to 3.3V)

**GPIO 34** IR pin sensor IN

**GPIO 35** Turret hall effect home sensor IN

**GPIO 39** Ball break sensor IN
