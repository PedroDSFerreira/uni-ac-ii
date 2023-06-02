#include <detpic32.h>

void configureAll(void);
void configureUART(int baud_rate, char parity, int data_bits, int stop_bits);
void putc(char byte2send);
volatile unsigned int count = 15;


int main(void)
{
    configureAll();
    
    EnableInterrupts();
    while(1);
}

void configureAll(void)
{
    configureUART(9600, 'O', 8, 2);

    // enable U2RXIE, disable U2TXIE (register IEC1)
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 1;
    // set UART2 priority level (register IPC8)
    IPC8bits.U2IP = 1;
    // clear Interrupt Flag bit U2RXIF (register IFS1)
    IFS1bits.U2TXIF = 0;
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2EIF = 0;
    // define RX interrupt mode (URXISEL bits)
    U2STAbits.URXISEL = 0;
    U2STAbits.UTXISEL = 0;

    

    // configure LEDS
    TRISE = TRISE & 0xFFE1; // 4-1
    LATE = LATE | 0x007E;
    
}

void configureUART(int baud_rate, char parity, int data_bits, int stop_bits)
{
    U2MODEbits.ON = 0;              // desativa a UART
    U2MODEbits.BRGH = 0;            // configura fator de sobre amostragem (0 ou 1)
    U2BRG = (PBCLK + (8 * baud_rate))/(16 * baud_rate) - 1;
    switch (parity)
    {
        case 'N':
            U2MODEbits.PDSEL = 0;
            break;
        case 'E':
            U2MODEbits.PDSEL = 1;
            break;
        case 'O':
            U2MODEbits.PDSEL = 2;
            break;
        default:
            U2MODEbits.PDSEL = 0;
            break;

    }
    U2MODEbits.PDSEL = 0;           // 0 (8N), 1 (8E), 2 (8O), 3 (9N) --- ver manual
    U2MODEbits.STSEL = stop_bits -1; // 0 (1 stop bits), 1 (2 stop bits) --- ver manual
    U2STAbits.UTXEN = 1;            // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 1;            // ativa receção (ver nota abaixo)
    U2MODEbits.ON = 1;              // ativa UART
}

void putc(char byte2send)
{
    while (U2STAbits.UTXBF);
    U2TXREG = byte2send;
} 


void _int_(32) isr_uart2(void)
{
    
    if (IFS1bits.U2RXIF)
    {
        static char* msg= "RESET";
        // Read character from FIFO (U2RXREG)
        // Send the character using putc()
        char c = U2RXREG;
        putc(c);
        switch (c)
        {
            case 'U':
                count = (count + 1) % 16;
                break;
            case 'R':
                count = 0;
        
                int i;
                for( i = 0; i < 5; i++ ) { 
                    putc(msg[i]);
                }
                break;
            default:
                break;
        }
        LATE = (LATE & 0xFFE1) | (count << 1);
        // Clear UART2 Rx interrupt flag
        IFS1bits.U2RXIF = 0;
    }
} 