#include <detpic32.h>

void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configureAll(void);


static const char disp7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
volatile int voltage = 0; // Global variable

int main(void)
{

    configureAll();

    // Configure interrupt system
    EnableInterrupts(); // Global Interrupt Enable

    // Reset flags
    IFS1bits.AD1IF = 0; // ADC
    IFS0bits.T1IF = 0;  // Timer 1
    IFS0bits.T3IF = 0;  // Timer 3
    
    while(1); // all activity is done by the ISR
    return 0;
} 

void configureAll(void)
{
    // ADC
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts 
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag

    TRISBbits.TRISB4 = 1;       // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0;       // RBx configured as analog input
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this
                                // mode an internal counter ends sampling and
                                // starts conversion
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter
                                // interrupt is generated. At the same time,
                                // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8-1;     // Interrupt is generated after N samples
                                // (replace N by the desired number of
                                // consecutive samples)
    AD1CHSbits.CH0SA = 4;       // replace x by the desired input
                                // analog channel (0 to 15)
    AD1CON1bits.ON = 1;         // Enable A/D converter
                                // This must the last command of the A/D
                                // configuration sequence

    // Timer 1
    T1CONbits.TCKPS = 2;    // 5Hz
    PR1 = 31249;            
    TMR1 = 0;               // Clear timer T1 count register
    T1CONbits.TON = 1;      // Enable timer T1 (must be the last command of the
                            // timer configuration sequence) 

    // Timer 3
    T3CONbits.TCKPS = 2;    // 100Hz
    PR3 = 49999;            
    TMR3 = 0;               // Clear timer T3 count register
    T3CONbits.TON = 1;      // Enable timer T3 (must be the last command of the
                            // timer configuration sequence) 


    // Interrupt priorities
    IPC6bits.AD1IP = 2; // ADC
    IPC1bits.T1IP = 2;  // Timer 1
    IPC3bits.T3IP = 2;  // Timer 3

    // Enable interrupts
    IEC1bits.AD1IE = 1; // ADC
    IEC0bits.T1IE = 1;  // Timer 1
    IEC0bits.T3IE = 1;  // Timer 3


    // 7 seg displays
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;
}

void _int_(27) isr_adc(void)
{
    // read vals
    int mean = 0;
    int i;
    int *p = (int *)(&ADC1BUF0);
    for( i = 0; i < 8; i++ ) { 
        mean += p[i*4]; 
    }
    // Calculate voltage amplitude
    mean = mean/8;

    // Convert voltage amplitude to decimal and store the result in the global variable "voltage"
    voltage = (mean*33+511)/1023;

    
    // Reset AD1IF flag
    IFS1bits.AD1IF = 0;             
} 

void _int_(4) isr_T1(void) 
{
    // Start A/D conversion
    AD1CON1bits.ASAM = 1;

    // Reset T1IF flag
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void) 
{
    // Send "voltage" value to displays
    send2displays(toBcd(voltage));

    // Reset T3IF flag
    IFS0bits.T3IF = 0;
}

void send2displays(unsigned char value)
{
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

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
} 