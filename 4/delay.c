#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int ms) 
{
    unsigned int wait = ms * CLOCK;
    resetCoreTimer();
    while(readCoreTimer() < wait);
}