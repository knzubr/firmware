
/**
 * \file
 *         Network interface and configuration 
 */

/*
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 */

#include "uip-netif.h"

#include <string.h>
#include <stdlib.h>
#include "include/uip-netif.h"



/*---------------------------------------------------------------------------*/
/** \brief The single physical interface */
struct uip_netif *uip_netif_physical_if;



static const uip_ipaddr_t solicited_prefix =
  {{ 0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0xff, 0, 0, 0 }}; 
/*---------------------------------------------------------------------------*/
/*
u8_t
uip_netif_is_addr_my_solicited(uip_ipaddr_t *ipaddr)
{
  if (memcmp(ipaddr, &solicited_prefix, 13) != 0)
    return 0;

  ipaddr = (uip_ipaddr_t *)(((char *)ipaddr) + 13);
  for(i = 1; i < UIP_CONF_NETIF_MAX_ADDRESSES; i++) {
    if((uip_netif_physical_if.addresses[i].state != PREFERRED))
      continue;

    if (memcmp(ipaddr,
      ((char *)&uip_netif_physical_if.addresses[i].ipaddr) + 13, 3) == 0)
      return 1;
  }
  return 0;
#if 0
  if(uip_ipaddr_cmp(ipaddr, &uip_netif_physical_if.solicited_node_mcastaddr))
    return 1;
  return 0;
#endif
}

/*---------------------------------------------------------------------------*/
struct uip_netif_addr *
uip_netif_addr_lookup(uip_ipaddr_t *ipaddr, u8_t length, uip_netif_type type) {
  /*
  for(i = 0; i < UIP_CONF_NETIF_MAX_ADDRESSES; i ++) {
    if((uip_netif_physical_if.addresses[i].state != NOT_USED) &&
       (uip_netif_physical_if.addresses[i].type == type || type == 0) &&
       (uip_ipaddr_prefixcmp(&(uip_netif_physical_if.addresses[i].ipaddr), ipaddr, length))) { 
      return &uip_netif_physical_if.addresses[i]; 
    }
  }
  */
  return &uip_netif_physical_if->addresses[0];
  
  return NULL; 
}


/** @} */
