#include <detpic32.h>

static const char disp7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
volatile int temperature = 0; // Global variable

void configureAll(void);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void delay(unsigned int ms);

int main(void) 
{
    configureAll();
    EnableInterrupts();

    while(1) {
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 );
        int mean = 0;
        int i;
        int *p = (int *)(&ADC1BUF0);
        for( i = 0; i < 2; i++ ) { 
            mean += p[i*4]; 
        }
        mean = mean/2;
        temperature = ((mean*(65-15)+511)/1023)+15;
        delay(100);
    }
    return 0;
}

void configureAll(void)
{
    // Configure 7seg display
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;

    // Configure ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1; 
    AD1CON3bits.SAMC = 16;  
    AD1CON2bits.SMPI = 2-1; 
    AD1CHSbits.CH0SA = 4;   
    AD1CON1bits.ON = 1;   

    // Configure timer 2
    // int fout = 120
    // int kprescaler = (PBCLK/(65535+1)*fout);
    T2CONbits.TCKPS =  2; // 4
    PR2 = 41666;
    TMR2 = 0;
    T2CONbits.TON = 1;

    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
}

void _int_(8) isr_T2(void)
{
    // Send "temperature" value to displays
    send2displays(toBcd(temperature));

    // Reset T2IF flag
    IFS0bits.T2IF = 0;
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

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer()<20000*ms);
}