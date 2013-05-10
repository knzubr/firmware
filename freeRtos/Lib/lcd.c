#include<lcd.h>
// PB5 D4, PB2 D7
unsigned char odwroc(unsigned char dana)
{
	unsigned char pomoc=0xFF;
	if(dana&0x01) pomoc|=0x80;
	else pomoc&=~0x80;
	if(dana&0x02) pomoc|=0x40;
	else pomoc|=~0x40;
	if(dana&0x04) pomoc|=0x20;
	else pomoc&=~0x20;
	if(dana&0x08) pomoc|=0x10;
	else pomoc&=~0x10;
	if(dana&0x10) pomoc|=0x08;
	else pomoc&=~0x08;
	if(dana&0x20) pomoc|=0x04;
	else pomoc&=~0x04;
	if(dana&0x40) pomoc|=0x02;
	else pomoc&=~0x02;
	if(dana&0x80) pomoc|=0x01;
	else pomoc&=~0x01;

	return pomoc;	
}
void polbajt(unsigned char data)
{
  if(data&0x01)
  {
	  PORTB.OUT|=LCD_D4;
  }
  else
  {
	  PORTB.OUT&=~LCD_D4;
  }

  if(data&0x02)
  {
	  PORTB.OUT|=LCD_D5;
  }
  else
  {
	  PORTB.OUT&=~LCD_D5;
  }

  if(data&0x04)
  {
	  PORTB.OUT|=LCD_D6;
  }
  else
  {
	  PORTB.OUT&=~LCD_D6;
  }

  if(data&0x08)
  {
	  PORTB.OUT|=LCD_D7;
  }
  else
  {
	  PORTB.OUT&=~LCD_D7;
  }
}
void lcdwritecommand(unsigned char command)
{
LCD_RS_clear;
lcdWrite(command);
}
void lcdWrite(unsigned char data)
{
uint8_t pomoc=odwroc(data>>4);
data=odwroc(data);
LCD_E_set;
polbajt(pomoc);
LCD_E_clear;
LCD_E_set;
polbajt(data);
LCD_E_clear;
_delay_us(50);
}
void lcdsend(unsigned char data)
{
LCD_RS_set;
lcdWrite(data);
}
void lcdclear(void)
{
lcdwritecommand(0x01);
_delay_ms(2);
}
void lcdinit(void)
{// PB5 D4, PB2 D7
portENTER_CRITICAL();
  {
    xLCDrec = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
  }
  portEXIT_CRITICAL();
	PORTB.DIR|=(PIN5_bm|PIN4_bm|PIN3_bm|PIN2_bm);
	PORTA.DIR|=(PIN1_bm|PIN2_bm);
	LCD_E_set;
	LCD_RS_set;
	_delay_ms(15);
	LCD_RS_clear;
	LCD_E_clear;
	for(int i = 0; i < 3; i++) 
	{
	   LCD_E_set;
	   PORTB.OUT&=0xF3;
	   LCD_E_clear;	
	   _delay_ms(5); // czekaj 5ms
	}
	LCD_E_set;
	polbajt(0x02); //tryb 4 bitowy
	LCD_E_clear;
	_delay_ms(1); // czekaj 1ms 
	lcdwritecommand(0x20|0|8|0);
	lcdwritecommand(0x80|0);
	lcdwritecommand(0x01);
	_delay_ms(2); // czekaj 2ms 
	lcdwritecommand(0x04|0|2);
	lcdwritecommand(0x08|4|2|1);	
}
