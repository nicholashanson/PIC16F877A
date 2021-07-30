#include <htc.h>

#define _XTAL_FREQ 4000000

char UART_init(const long int baudrate)
{
    unsigned int x;
    x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);   
    if (x > 255)                                    
    {
        x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); 
        BRGH = 1;                                     
    }
    if (x < 256)
    {
        SPBRG = x;                                  
        SYNC = 0;                       // asynchronous mode
        SPEN = 1;                       // enable serial port
        TRISC7 = 1;                                  
        TRISC6 = 1;                                
        CREN = 1;                       // enable continuous reception
        TXEN = 1;                       // enable transmission
        return 1;       
    }
    return 0;                     
}

char UART_TX_empty()
{
    return TRMT;
}

void UART_write(char data)
{
    while (!TRMT);
    TXREG = data;
}

void UART_write_text(char * text)
{
    int i;
    for (i=0; text[i]!='\0'; i++)
    UART_write(text[i]);
}

void I2C_master_init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISC3 = 1;
    TRISC4 = 1;
}

void I2C_master_wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_master_start()
{
    I2C_master_wait();
    SEN = 1;
}

void I2C_master_repeated_start()
{
    I2C_master_wait();
    RSEN = 1;
}

void I2C_master_stop()
{
    I2C_master_wait();
    PEN = 1;
}

void I2C_master_write(unsigned d)
{
    I2C_master_wait();
    SSPBUF = d;
}

unsigned short I2C_master_read(unsigned short a)
{
    unsigned short temp;
    I2C_master_wait();
    RCEN = 1;
    I2C_master_wait();
    temp = SSPBUF;
    I2C_master_wait();
    ACKDT = (a) ? 0 : 1;
    ACKEN = 1;
    return temp;
}

void main()
{
    nRBPU = 0;
    TRISB = 0xFF;;
    TRISD = 0x00;
    PORTD = 0x00;
    I2C_master_init(100000);
    while (1)
    {
        I2C_master_start();
        I2C_master_write(0x30);
        I2C_master_write(PORTB);
        I2C_master_stop();
        __delay_ms(200);
        I2C_master_start();
        I2C_master_write(0x31);
        PORTD = I2C_master_read(0);
        I2C_master_stop();
        __delay_ms(200);
    }
}

