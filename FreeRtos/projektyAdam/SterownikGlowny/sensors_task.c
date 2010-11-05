/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "sensors_task.h"
#include "memory_x.h"
#include "main.h"
#include "Rs485_prot.h"



void sensorsTaskInit(void)
{
  rollers = xmalloc(MAX_NUMBER_OF_ROLLERS * sizeof(struct sterRolet));
  memset(rollers, 0, MAX_NUMBER_OF_ROLLERS * sizeof(struct sterRolet));
}

void sensorsTask(void* pvParameters)
{
  pvParameters = NULL;
  uint8_t addr = 255;
  uint8_t i;

  for( ; ; )
  {
    addr++;
    
    if (addr == 0)
    {
      uint16_t tmp1 = MCP3008_getSampleSingle(0);
      voltage = (uint8_t)(tmp1>>5);
      vTaskDelay(10);
      tmp1 = MCP3008_getSampleSingle(1);
      tmp1 *=10;
      temperature = (uint8_t)(tmp1 / 24);
    
      vTaskDelay(10);

      continue;
    }
    //continue;

    struct sterRolet *tmp = NULL;
    for (i=0; i< MAX_NUMBER_OF_ROLLERS; i++)
      if (rollers[i].rsDeviceState.adres == addr)
        tmp = &rollers[i];
    
    if (rs485ping(addr)==0)
    {
      if (tmp == NULL)
      {
        for (i=0; i< MAX_NUMBER_OF_ROLLERS; i++)
        {
          if (rollers[i].rsDeviceState.adres == 0)
          {
            tmp = &rollers[i];
            tmp->rsDeviceState.adres = addr;
            break;
          }
        }
      }
      if (tmp != NULL)
      {
        tmp->rsDeviceState.stan &= (~NOT_DETECTED);
        rs485hello(addr, (void *)(tmp->rsDeviceState.version), 7);
      }
    }
    else
    {
      if (tmp != NULL)
      {
        if (tmp->rsDeviceState.stan & NOT_DETECTED)
        {
          tmp->rsDeviceState.adres = 0;
        }
        tmp->rsDeviceState.stan |= NOT_DETECTED;
      }
    }

    vTaskDelay(10);
  }
}
