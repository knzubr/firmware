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
#include "netstack_task.h"


void encTask ( void *pvParameters )
{
  FILE *netstackDebug = (FILE *) pvParameters;
  uint16_t plen;

  nicInit();
  ipInit();
  arpInit();
  icmpInit();

  for ( ; ; )
  {
    vTaskDelay ( 0 );
    
    // get the next new packet:
    plen = nicPoll();
    /*plen will ne unequal to zero if there is a valid
    * packet (without crc error) */
    if ( plen==0 )
    {
      flushUdpQueues();
      flushTcpQueues();
      //flush HTTP long file queue 
      continue;
    }
    
    if(nicState.layer2.ethHeader->type == htons(ETHTYPE_IP))             // process an IP packet
    {
      arpIpIn();
      netstackIPv4Process();
    }
    else if(nicState.layer2.ethHeader->type == htons(ETHTYPE_ARP))       // process an ARP packet
    {
      arpArpIn();
    }
    else
    {
      if (netstackDebug != NULL)
      {
        fprintf_P(netstackDebug, PSTR("Unknown packet\r\n"));
      }
    }
    
    continue;
  }
}
