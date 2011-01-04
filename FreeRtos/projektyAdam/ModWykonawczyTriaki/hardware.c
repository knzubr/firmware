#include "hardware.h"


void hardwareInit(void)
{
  DDRB  = 0x07; //0 - P3  , 1 - P2  , 2 - P1
  PORTB = 0x38; //3 - Adr3, 4 - Adr4, 5 - Adr5
  DDRC  = 0x00; //0 - Adr0, 1 - Adr1, 2 - KlRolety1Up, 3 - klRolety1Down
  PORTC = 0x3F; //4 - KlRolety2Up, 5 - KlRolety2Down
  DDRD  = 0xEE; //0 - RXD, 1 - TXD, 2 - !RxEn, 3 - TxEn, 
  PORTD = 0x00; //5 - Led1, 6 - Led2, 7 - P4)
  /*Ustalanie adresu
    bit 7, 6 = 0 dla sterowników rolet i światła
   */
  adres =  (PINB & 0x38) >> 1;
  adres |= (PINC & 0x03);
}

inline void roletawGore(void)
{
  PORTB &= ~0x02;
  PORTB |=  0x04;
}

inline void roletawDol(void)
{
  PORTB &= ~0x04;
  PORTB |=  0x02;	
}

inline void roletaStop(void)
{
  PORTB &= ~0x06;
}

inline char czytKlawiszRolwGore(void)
{
  return PINC & 0x04;
}

inline char czytKlawiszRolwDol(void)
{
  return PINC & 0x08;
}
