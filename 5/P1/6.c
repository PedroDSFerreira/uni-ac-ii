#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int);
void send2displays(unsigned char value);

int main(void) 
{ 
    int i;
    int counter = 0;

    // Configure ports RB14-RB8 as output
    TRISB = TRISB & 0x80FF;
    // Configure ports RD6-RB5 as output
    TRISD = TRISD & 0xFF9F;
    

    while(1) 
    { 
        i = 0;

        do
        {
            send2displays(counter);
            // wait 100Hz
            delay(10);
        } while(i++ < 20);
        
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

void send2displays(unsigned char value)
{
    unsigned char disp7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;    // static variable: doesn't loose its
                                    // value between calls to function
    unsigned int digit_low = value & 0x0F;
    unsigned int digit_high = value >> 4;

    // if "displayFlag" is 0 then send digit_low to display_low
    if (displayFlag == 0)
    {
        LATB = (LATB & 0x80FF) | (disp7Scodes[digit_low] << 8);
        LATD = (LATD & 0xFF9F) | 0x0020;
    }
    // else send digit_high to didplay_high
    else
    {
        LATB = (LATB & 0x80FF) | (disp7Scodes[digit_high] << 8);
        LATD = (LATD & 0xFF9F) | 0x0040;
    }

    // toggle "displayFlag" variable
    displayFlag = displayFlag ? 0 : 1;
}