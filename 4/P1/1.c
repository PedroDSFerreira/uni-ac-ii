#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int);

int main(void) 
{ 
    // Configure port RC14 as output
    TRISCbits.TRISC14 = 0;

    
    while(1) 
    { 
        // Wait 0.5s
        delay(500);
        // Toggle RC14 port value
        LATCbits.LATC14 =  !LATCbits.LATC14;
    } 
    return 0; 
}

void delay(unsigned int ms) 
{
    unsigned int wait = ms * CLOCK;

    resetCoreTimer();
    while(readCoreTimer() < wait);
}
