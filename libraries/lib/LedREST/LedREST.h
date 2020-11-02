#ifndef LedREST_h
#define LedREST_h

#include "Arduino.h"

class LedREST
{
  public:
    LedREST(int pin);
    boolean on();
    boolean off();
	boolean state();
  private:
	boolean ledState;
    int _pin;
};

#endif