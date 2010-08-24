#include "hardware.h"
#include <avr/io.h>

void hardwareInit(void)
{
  //DDRA = 0x00;  //External Memory
  DDRB = 0xF7;
  PORTB = 0xF1;
  /*
   0 - Sl_RST
   1 - SCK
   2 - MOSI
   3 - MISO
   4 - External SPI ASR 4
   5 - External SPI ASR 5
   6 - External SPI ASR 6
   7 - External SPI ASR 7
  */
  
  //DDRC = 0x00;  //External Memory

  DDRD = 0x00;
  /*
   0 - SCL
   1 - SDA
   2 - RxD USB
   3 - TxD USB
   4 - External SPI ASR 0
   5 - External SPI ASR 1
   6 - External SPI ASR 2
   7 - External SPI ASR 3
   */

  DDRE  = 0x0E;
  PORTE = 0x0C;
  /*
   0 - RxD Rs485
   1 - TxD Rs485
   2 - ENC RST
   3 - ENC CS
   4 - INT 4
   5 - INT 5
   6 - INT 6
   7 - INT Enc28j60
  */
  DDRF = 0x00;    //JTAG and A/C
  DDRG = 0x1F;
  /*
   0 - WR
   1 - RD
   2 - ALE
   3 - SD CS
   4 - RS485 TxEn
   5 - 
   6 - 
   7 - 
   */
}
