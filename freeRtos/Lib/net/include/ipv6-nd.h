#ifndef IPV6_ND_H
#define IPV6_ND_H 

/**
 * @file ipv6-nd.h 
 * @brief IP Network Discovery 
 * 
 * Please Notice, that we do have a fwe strategies - Router Discovery/Solicitation, Neighbour Discovery/Solicitation
 * For now, only Neighbour Discovery is implemented
 */

#include "net.h"
#include "softwareConfig.h"

#define uip_eth_addr netEthAddr //tymczasowo, docelowo zamien na zmienna z liku net.h
#define MAX_TIME 128
#define ENTRIES 8


typedef struct {
  uip_ipaddr_t ipaddr;
  struct netEthAddr addr;
  uint32_t time; ///Idea - reference time measuerd in "tick count"
}neighbor_entry;

neighbor_entry* entries;


void vNDinit(void); //init ND cache

void uip_neighbor_add(uip_ipaddr_t *ipaddr, struct netEthAddr *addr);

neighbor_entry * find_entry(uip_ipaddr_t *ipaddr);

#if ND_DEBUG
/** Debug Network Discovery */
FILE *debugstream; 

/** Assign debug stram */
void vNDDebugInit(FILE *stream); 

#endif /**ND_DEBUG*/


#endif /**IPV6_ND_H*/