#include <detpic32.h>
#define CLOCK (PBCLK/1000)

int main(void) 
{ 
    // Configure ports RB14-RB8 as output
    TRISB = TRISB & 0x80FF;
    // Configure ports RD6-RB5 as output
    TRISD = TRISD & 0xFF9F;

    // Set output vals (RD6=0, RD5=1)
    LATD = (LATD & 0xFFBF) | 0x0020;

    int val;
    while(1) 
    { 
        val = getChar();

        // Check if char is between 'a' and 'g'
        if(val >= 'a' && val <= 'g')
        {
            // Convert char to int
            val = val - 'a';
            // Set RB14-RB8
            LATB = (LATB & 0x80FF) | (0x1 << 8 + val);
        }
        // Check if char is between 'A' and 'G'
        else if(val >= 'A' && val <= 'G')
        {
            // Convert char to int
            val = val - 'A';
            // Set RB14-RB8
            LATB = (LATB & 0x80FF) | (0x1 << 8 + val);
        }
    } 
    return 0; 
}
