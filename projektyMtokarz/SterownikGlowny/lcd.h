#ifndef LCD_H
#define LCD_H
// PB5 D4, PB2 D7
#include<avr/io.h>
#include <util/delay.h>
#define LCD_RS_set PORTA.OUT|=0x04;
#define LCD_RS_clear PORTA.OUT&=0xFB;
#define LCD_E_set PORTA.OUT|=0x03;
#define LCD_E_clear PORTA.OUT&=0xFD;
//#define LCD_DATA PORTB.0UT;
#define LCD_D4 0x20;
#define LCD_D5 0x10;
#define LCD_D6 0x08;
#define LCD_D7 0x04;
unsigned char odwroc(unsigned char dana);
void polbajt(unsigned char data);
void lcdint(void);
void lcdwritecommand(unsigned char command);
void lcdWrite(unsigned char data);
void lcdclear(void);
void lcdsend(unsigned char data);
#endif