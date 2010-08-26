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

void Ds1305_init(uint8_t (*spiSendFunc)(uint8_t), void (*spiEnableDS1305Func)(void), void (*spiDisableDS1305Func)(void))
{
  DS1305_global.spiSend          = spiSendFunc;
  DS1305_global.spiEnableDS1305  = spiEnableDS1305Func;
  DS1305_global.spiDisableDS1305 = spiDisableDS1305Func;
}

void readTimeBCD(timeBCD_t *time)
{
  spiTake();
  DS1305_global.spiEnableDS1305();
  
  uint8_t *ptr = (uint8_t *)(time);
  uint8_t i;
  
//  spiSetCPHA();
  DS1305_global.spiSend(0x00);
//  spiSetCPHA();
  for (i=0; i<sizeof(timeBCD_t); i++)
  {
    *ptr = DS1305_global.spiSend(i);
    ptr++;
  }  
  DS1305_global.spiDisableDS1305();  
  spiGive();
//  spiClearCPHA();
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
  spiTake();
  DS1305_global.spiEnableDS1305();
  
//  spiSetCPHA();
  uint8_t *ptr = (uint8_t *)(time);
  uint8_t i;
  DS1305_global.spiSend(0x80);
  for (i=0; i<sizeof(timeBCD_t); i++)
  {
    DS1305_global.spiSend(*ptr);
    ptr++;
  }  
  
  DS1305_global.spiDisableDS1305();  
//  spiClearCPHA();
  spiGive();
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

void ds1305start(void)
{  
  spiTake();
//  spiSetCPHA();
  DS1305_global.spiEnableDS1305();

  DS1305_global.spiSend(0x8F);
  DS1305_global.spiSend(0x00);
  
  DS1305_global.spiDisableDS1305();  
//  spiClearCPHA();
  spiGive();
  return 0;
}


uint8_t ds1305writeMem      (uint8_t addr, uint8_t length, uint8_t *data)
{
  if (addr > 95)
    return 1;
  if (addr + length > 95)
    return 2;

  addr += 0xA0;
  
  spiTake();
  DS1305_global.spiEnableDS1305();

  DS1305_global.spiSend(addr);
  while (length > 0)
  {
    DS1305_global.spiSend(*data);
    data++;
    length--;
  }
  
  DS1305_global.spiDisableDS1305();  
  spiGive();
  return 0;
}
uint8_t ds1305readMem       (uint8_t addr, uint8_t length, uint8_t *data)
{
  if (addr >95)
    return 1;
  if (addr + length > 95)
    return 2;
  
  addr += 0x20;
  
  spiTake();
  DS1305_global.spiEnableDS1305();

  DS1305_global.spiSend(addr);
  while (length > 0)
  {
    *data = DS1305_global.spiSend(0);
    data++;
    length--;
  }
  
  DS1305_global.spiDisableDS1305();  
  spiGive();
  return 0;
}
