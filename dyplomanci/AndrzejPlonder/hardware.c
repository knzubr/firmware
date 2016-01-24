#include "hardware.h"

#if LANG_EN
#include "hardware_en.h"
#endif

#if LANG_PL
#include "hardware_pl.h"
#endif

xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
xQueueHandle      xSpiRxEnc;
// napisać funkcje do konfiguracji zew pam s-ram

//#include<avr/iox128a1.h>
void hardwareInit(void)
{
  //DDRA = 0x00;  //External Memory

 //DDRA = 0x00;  //External Memory
  PORTA.DIR=0x03;
  PORTA.OUT=0x00;
/** Port B
 * 7   - JTAG TD0
 * 6   - JTAG TCK
 * 5   - JTAG TDI
 * 4   - JTAG TMS
 * 5:2 - LCD D4:D7
 * 0 - ENC28J60 CS
 */
  PORTB.DIR=0x01;

/**
 * Port C
 * 7 - Rs485 kamery
 * 6 - Rs485 kamery
 * 5 - Rs485 włączanie nadajnika
 */
  PORTC.DIR=0xA0;

/* PORTD
7- SD SCK
6- SD S0
5- SD SI
4- ZL SPI SLRST
3- FT232 TXT-WY
2- FT232 RXT-WE
1- ENC28J60 RST
0- ENC28J60 INT
*/
  PORTD.DIR=0xBA;
/* PORTE
7:0- ZL SPI A7:A0
*/
  PORTE.DIR=0xFF;

/* PORTF
7- INT
6- INT
5- RS485 DE/RE
4- INT
3- RS485 TXD
2- RS485 RXD
*/
  PORTF.DIR=0x28;

/* PORTH
6:4- EBI A18:A16
2-   EBI ALE
1-   EBI RE
0-   EBI WE
*/
  PORTH.DIR=0xFF;

/**
 * PORTJ
 * 7:0- EBI D7:D0/A0:A7
 */
  PORTJ.DIR=0xFF;

/**
 * PORTK
 * 7:0- EBI A8:A15
 */
  PORTK.DIR=0xFF;
}

void Rs485TxStop_2(void)
{
  PORTF.OUT &= 0xDF;
}

void Rs485TxStart_2(void)
{
  PORTF.OUT |= 0x20;
}
