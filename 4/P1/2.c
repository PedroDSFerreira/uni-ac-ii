#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int);

int main(void) 
{ 
    // Configure port RE6-RE3 as output
    TRISE = TRISE & 0xFF87;
    int counter = 0;
    while(1) 
    { 
        LATE = (LATE & 0xFF87) | (counter << 3);
        // Wait 4Hz
        delay(250);
        
        counter++;
    } 
    return 0; 
}

void delay(unsigned int ms) 
{
    unsigned int wait = ms * 20000;
    resetCoreTimer();
    while(readCoreTimer() < wait);
}