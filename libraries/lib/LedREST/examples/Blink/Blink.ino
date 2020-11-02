#include <LedREST.h>

LedREST led(13);

void setup()
{
}

void loop()
{
  led.on(); 
  delay(3000);
  led.off();
}