/*
This library configures the pin connected to ATMEGA2560 so that it can read
the PPM signal sent from the ATMEGA32U4.
This library aims to be as "low-level" as possible so that by default the platform
will always function as an effective servo signal gateway.

Although the are lots of tutorials about timers and interrupts online, when using
a new device often times I like to consult the avr-libc user manual!

Excellent example here:
https://stackoverflow.com/questions/31852914/read-rc-pwm-signal-using-atmega2560-in-atmel-avr-studio
*/

#ifndef _APM_PPM_H
#define _APM_PPM_H

#include "Arduino.h"

const int ppmIn = 48; //PL_1
const int debugOut = 5;
const int numChannel = 8;


class APM_PPM{
  public:
    APM_PPM();
    void initialize();


  private:
};

#endif
