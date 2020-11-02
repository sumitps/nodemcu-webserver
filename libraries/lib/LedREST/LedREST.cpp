#include "Arduino.h"
#include "LedREST.h"

LedREST::LedREST(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

boolean LedREST::on()
{
  digitalWrite(_pin, HIGH);
  ledState = true;
  return ledState;
}

boolean LedREST::off()
{
  digitalWrite(_pin, HIGH);
  ledState = false;
  return ledState;
}