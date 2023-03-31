#include <detpic32.h>
#define CLOCK (PBCLK/1000)

void delay(unsigned int);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

volatile unsigned char voltage = 0; // Global variable


int main(void)
{
    unsigned int cnt = 0;
    // Configure all (digital I/O, analog input, A/D module, interrupts)

    // Digital I/O
    // Configure ports RB14-RB8 as output
    TRISB = TRISB & 0x80FF;
    // Configure ports RD6-RB5 as output
    TRISD = TRISD & 0xFF9F;

    // A/D module
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

    // Interrupts
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts 
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts

    EnableInterrupts(); // Global Interrupt Enable
    while(1)
    {
        if(cnt == 0) // 0, 200 ms, 400 ms, ... (5 samples/second)
        {
            // Start A/D conversion
            AD1CON1bits.ASAM = 1;
        }
        // Send "voltage" value to displays
        send2displays(toBcd(voltage));
        
        cnt = (cnt + 1) % 20;
        // Wait 10 ms
        delay(10);
    }
    return 0;
}
void _int_(27) isr_adc(void)
{
    // Read 8 samples (ADC1BUF0, ..., ADC1BUF7) and calculate average
    int *p = (int *)(&ADC1BUF0);
    int i, mean;
    int val = 0;
    for(i = 0; i < 8; i++ ) {
        val += p[i*4];
    }

    // Calculate voltage amplitude
    mean = val/8;

    // Convert voltage amplitude to decimal and store the result in the global variable "voltage"
    voltage = (mean*33+511)/1023;
    // Reset AD1IF flag
    IFS1bits.AD1IF = 0; 
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

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
} 