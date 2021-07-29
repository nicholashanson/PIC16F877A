#include <htc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _XTAL_FREQ          4000000

#define LCD_DELAY           5 
#define LCD_START_UP        15

#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_CURSOR_BACK     0x10
#define LCD_CURSOR_FWD      0x14
#define LCD_PAN_LEFT        0x18
#define LCD_PAN_RIGHT       0x1C
#define LCD_CURSOR_OFF      0x0C
#define LCD_CURSOR_ON       0x0E
#define LCD_CURSOR_BLINK    0x0F
#define LCD_CURSOR_LINE2    0xC0

#define FUNCTION_SET        0x28
#define ENTRY_MODE          0x06 
#define DISPLAY_SETUP       0x0C 

#define INSTR               0
#define DATA                1

#define LCD_PORT            PORTD
#define LCD_PWR             PORTDbits.RD7
#define LCD_EN              PORTDbits.RD6
#define LCD_RW              PORTDbits.RD5
#define LCD_RS              PORTDbits.RD4 

#define NB_LINES            2 
#define NB_COL              16 

void LCD_initialize(void);
void LCD_put_char(char c);
void LCD_put_cmd(char c);
void LCD_put_str(const char * str); 
void LCD_write_nibble(char ch, char rs);
void LCD_go_to(char pos, char ln);

void main(void) 
{
    LCD_initialize();

    LCD_put_str("Hi");
    LCD_go_to(0,1);
    LCD_put_str("I'm Nick");  
    while (1)
    {
    }
}

void LCD_initialize()
{
    LCD_PORT = 0;
    TRISD = 0x00;
    LCD_PWR = 1;
    __delay_ms(LCD_START_UP);

    LCD_put_cmd(0x32);

    LCD_put_cmd(FUNCTION_SET);

    LCD_put_cmd(DISPLAY_SETUP);

    LCD_put_cmd(LCD_CLEAR);

    LCD_put_cmd(ENTRY_MODE);
}


void LCD_write_nibble(char c, char rs)
{
    c = (c >> 4);

    c = (c & 0x0F);

    LCD_PORT = (LCD_PORT & 0xF0);
    
    LCD_PORT = (LCD_PORT | c);

    LCD_RS = rs;

    LCD_RW = 0;

    LCD_EN = 1;

    LCD_EN = 0;
}

void LCD_put_char(char c)
{
    __delay_ms(LCD_DELAY);

    LCD_write_nibble(c,DATA);

    c = (c << 4);

    LCD_write_nibble(c,DATA);
}


void LCD_put_cmd(char c)
{
    __delay_ms(LCD_DELAY);

    LCD_write_nibble(c,INSTR);

    c = (c << 4);

    __delay_ms(1);

    LCD_write_nibble(c,INSTR);
}


void LCD_put_str(const char * str)
{
    char i=0;

    while (str[i])
    {
        LCD_put_char(str[i++]);
    }
}

void LCD_go_to(char pos,char ln)
{
    if ((ln > (NB_LINES-1)) || (pos > (NB_COL-1)))
    {
        return;
    }
    
    LCD_put_cmd((ln == 1) ? (0xC0 | pos) : (0x80 | pos));

    __delay_ms(LCD_DELAY);
}

