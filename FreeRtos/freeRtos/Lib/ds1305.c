/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 * http://www.gnu.org/licenses/gpl.html
 *
 * Based on the enc28j60.c file from the AVRlib library by Pascal Stang.
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "ds1305.h"
#include "spi.h"

//
#define waitspi() while(!(SPSR&(1<<SPIF)))

void readTimeBCD(timeBCD_t *time)
{
  enableExternalSpiDevice(DS1305_CS);


  disableAllDevices();
}

#if USE_DECODED_TIME_STRUCT
void readTimeDecoded(timeDecoded_t *time)
{
  readTimeBCD((timeBCD_t *)(time));
}
void readTime (time_t *time)
{
  readTimeBCD((timeBCD_t *)(time));
}
#endif /* USE_DECODED_TIME_STRUCT */


void setTimeBCD(timeBCD_t *time)
{
  enableExternalSpiDevice(DS1305_CS);


  disableAllDevices();
}

#if USE_DECODED_TIME_STRUCT
void setTimeDecoded(timeDecoded_t *time)
{
  setTimeBCD((timeBCD_t *)(time));
}
void setTime(time_t *time)
{
  setTimeBCD((timeBCD_t *)(time));
}
#endif /* USE_DECODED_TIME_STRUCT */

int8_t writeMem      (uint8_t addr, uint8_t length, uint8_t *data)
{
  enableExternalSpiDevice(DS1305_CS);

  disableAllDevices();
  return -2;
}
int8_t readMem       (uint8_t addr, uint8_t length, uint8_t *data)
{
  enableExternalSpiDevice(DS1305_CS);


  disableAllDevices();
  return -2;
}
