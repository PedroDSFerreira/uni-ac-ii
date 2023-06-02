#include <detpic32.h>

volatile int dutyCycle = 25;
configureAll(void);
void delay(unsigned int ms);

int main(void) 
{
    configureAll();
    while(1)
    {
        delay(250);
        if (PORTBbits.RB3 == 0 && PORTBbits.RB0 == 1)
        {
            dutyCycle = 25;
        }
        else if (PORTBbits.RB3 == 1 && PORTBbits.RB0 == 0)
        {
            dutyCycle = 70;
        }

        OC1RS = ((PR2 + 1) * dutyCycle)/100;

    }
}

void configureAll(void)
{
    // Timer 2
    T2CONbits.TCKPS = 2; // 4
    PR2 = 33332;   // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz 
    TMR2 = 0;    // Reset timer T2 count register 
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the 
            //  timer configuration sequence) 
    // OC2
    OC1CONbits.OCM = 6;  // PWM mode on OCx; fault pin disabled 
    OC1CONbits.OCTSEL =0;// Use timer T2 as the time base for PWM generation 
    OC1RS = ((PR2 + 1) * dutyCycle)/100;   // Ton constant 
    OC1CONbits.ON = 1; // Enable OC1 module

    // switches
    LATB = LATB | 0x000A;
}

void delay(unsigned int us)
{
    resetCoreTimer();
    while(readCoreTimer()<20*us);
}