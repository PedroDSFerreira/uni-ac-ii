#include <detpic32.h>

void configureAll(void);
void setPWM(unsigned int dutyCycle);

int main(void)
{
    configureAll();

    EnableInterrupts(); // Global Interrupt Enable
    IFS0bits.T3IF = 0;  // Reset Timer 3 flag

    while(1) // all activity is done by the ISR
    {
        // Read the value of port RD0 and write it on port RC14
        LATCbits.LATC14 = PORTDbits.RD0;
    }
    return 0;

}


void configureAll(void)
{
    // Timer 3
    T3CONbits.TCKPS = 2;    // 100Hz
    PR3 = 49999;            
    TMR3 = 0;               // Clear timer T3 count register
    T3CONbits.TON = 1;      // Enable timer T3 (must be the last command of the
                            // timer configuration sequence) 

    IPC3bits.T3IP = 2;      // Interrupt priority
    IEC0bits.T3IE = 1;      // Enable interrupts

    // PWM
    OC1CONbits.OCM = 6;     // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1;  // Use timer T3 as the time base for PWM generation
    OC1RS = 12500;          // Ton constant
    OC1CONbits.ON = 1;      // Enable OC1 module

    // I/O
    TRISCbits.TRISC14 = 0;
    TRISDbits.TRISD0 = 1;   // LED
    TRISB = TRISB | 0x0004; // Toggle 3 as input


}

void setPWM(unsigned int dutyCycle)
{
    // duty_cycle must be in the range [0, 100]
    if (dutyCycle>=0 && dutyCycle<=100)
    {
        OC1RS = (dutyCycle*PR3)/100; // Determine OC1RS as a function of "dutyCycle"
    }
}



void _int_(12) isr_T3(void) 
{
    // Read value from switch and update ton constant
    int dutyCycle = PORTBbits.RB2 ? 100:5;
    setPWM(dutyCycle);

    // Reset T3IF flag
    IFS0bits.T3IF = 0;
}

