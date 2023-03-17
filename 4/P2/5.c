#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int);

int main(void) 
{ 
    unsigned char disp7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; // TODO
    int i;

    // Configure ports RB14-RB8 as output
    TRISB = TRISB & 0x80FF;
    // Configure ports RD6-RB5 as output
    TRISD = TRISD & 0xFF9F;
    // Set output vals (RD6=0, RD5=1)
    LATD = (LATD & 0xFFBF) | 0x0020;

    while(1) 
    { 
        for(i=0; i < 7; i++) 
        { 
            LATB = (LATB & 0x80FF) | (disp7Scodes[i] << 8);
            // wait 10Hz
            delay(100);
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
