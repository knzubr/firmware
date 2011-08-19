/**
 * @file ipv6-nd.c 
 * @brief IP Network Discovery 
 * 
 * Please Notice, that we do have a fwe strategies - Router Discovery/Solicitation, Neighbour Discovery/Solicitation
 * For now, only Neighbour Discovery is implemented
 */

#include "ipv6-nd.h"
#include "include/ipv6-nd.h"


void
vNDinit(void)
{
  entries = xmalloc( ENTRIES * sizeof(neighbor_entry));
  #if ND_DEBUG
  fprintf_P(debugstream, PSTR("ENTRIES %d %d:"), ENTRIES, sizeof(neighbor_entry));
  #endif
  
  for(uint8_t i = 0; i < ENTRIES; ++i) {
    entries[i].time = 0;
  }
}

#if ND_DEBUG
void vNDDebugInit(FILE *stream)
{
  debugstream = stream;
}
#endif/**ND_DEBUG*/


/*---------------------------------------------------------------------------*/
void
uip_neighbor_add(uip_ipaddr_t *ipaddr, struct netEthAddr *addr)
{
  uint8_t i, insertPos;
  uint32_t oldestTime = 0xFFFFFFFF;
  
 #if ND_DEBUG
  fprintf_P(debugstream, PSTR("Adding neighbor with link address %02x:%02x:%02x:%02x:%02x:%02x\n\r"),
	 addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3],
	 addr->addr[4], addr->addr[5]);
 
  
  fprintf_P(debugstream, PSTR("SizeofInt %d %d \n\r"),
	 sizeof(int), xTaskGetTickCount());
 #endif/**ND_DEBUG*/
  
 insertPos = 0;
  /* Find the first unused entry or the oldest used entry. */
 for(i = 0; i < ENTRIES; ++i) {
    if(entries[i].time == 0) {
      insertPos = i;
      break;
    }
    if(uip_ipaddr_cmp(&entries[i].ipaddr, ipaddr)) {
      insertPos = i;
      break;
    }
    if(entries[i].time < oldestTime) {
      oldestTime = entries[i].time;
      insertPos =i;
    }
 }
 
   fprintf_P(debugstream, PSTR("oldestTime %d \n\r"),oldestTime);
 
  /* Use the oldest or first free entry (either pointed to by the
     "oldestTime" variable). */
  entries[insertPos].time = (uint32_t)xTaskGetTickCount();
  uip_ipaddr_copy(&entries[insertPos].ipaddr, ipaddr);
  memcpy(&entries[insertPos].addr, addr, sizeof(struct netEthAddr));
  
 #if ND_DEBUG
  fprintf_P(debugstream, PSTR("EntryPacket[0]:\r\n"));
  for (i =0; i < sizeof(neighbor_entry); i++)
    fprintf_P(debugstream, PSTR("%02x:"), ((uint8_t *)entries)[i]);
  fprintf_P(debugstream, PSTR("\n\r"));
  
  fprintf_P(debugstream, PSTR("link address %02x:%02x:%02x:%02x:%02x:%02x\n\r"),
	 entries[0].addr.addr[0], entries[0].addr.addr[1], entries[0].addr.addr[2], entries[0].addr.addr[3],
	 entries[0].addr.addr[4], entries[0].addr.addr[5]);
  
  fprintf_P(debugstream, PSTR("IP address:\r\n"));
  fprintf_P(debugstream, PSTR("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x"),
	  entries[0].ipaddr.u8[0], entries[0].ipaddr.u8[1],
	  entries[0].ipaddr.u8[2], entries[0].ipaddr.u8[3],
	  entries[0].ipaddr.u8[4], entries[0].ipaddr.u8[5],
	  entries[0].ipaddr.u8[6], entries[0].ipaddr.u8[7],
	  entries[0].ipaddr.u8[8], entries[0].ipaddr.u8[9],
	  entries[0].ipaddr.u8[10], entries[0].ipaddr.u8[11],
	  entries[0].ipaddr.u8[12], entries[0].ipaddr.u8[13],
	  entries[0].ipaddr.u8[14], entries[0].ipaddr.u8[15]);
  fprintf_P(debugstream, PSTR("\r\n"));
 #endif/**ND_DEBUG*/
}
/*---------------------------------------------------------------------------*/
neighbor_entry *
find_entry(uip_ipaddr_t *ipaddr)
{
  int i;
  
  for(i = 0; i < ENTRIES; ++i) {
    if(uip_ipaddr_cmp(&entries[i].ipaddr, ipaddr)) {
      return &entries[i];
    }
  }
  return NULL;
}
/*---------------------------------------------------------------------------*/


