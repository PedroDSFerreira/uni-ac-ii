#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int);

int main(void) 
{ 
    unsigned char segment;
    int i;

    // Configure ports RB14-RB8 as output
    TRISB = TRISB & 0x80FF;
    // Configure ports RD6-RB5 as output
    TRISD = TRISD & 0xFF9F;
    // Set output vals (RD6=0, RD5=1)
    LATD = (LATD & 0xFFBF) | 0x0020;

    while(1) 
    { 
        segment = 1; 
        for(i=0; i < 7; i++) 
        { 
            // send "segment" value to display 
            LATB = (LATB & 0x80FF) | (segment << 8);
            // wait 0.5 second 
            delay(500);

            segment = segment << 1; 
        } 
        // toggle display selection
        LATD = LATD ^ 0x0060;
    } 
    return 0; 
}

void delay(unsigned int ms) 
{
    unsigned int wait = ms * 20000;
    resetCoreTimer();
    while(readCoreTimer() < wait);
}
