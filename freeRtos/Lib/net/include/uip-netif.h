 
#ifndef __UIP_NETIF_H__
#define __UIP_NETIF_H__


#ifndef UIP_CONF_NETIF_MAX_ADDRESSES
#define UIP_CONF_NETIF_MAX_ADDRESSES 2
#endif /*UIP_CONF_NETIF_MAX_ADDRESSES*/

#include "net.h"

/**
 * \brief Possible states for the address of an interface (RFC 4862 autoconf +
 * NOT_USED + INFINITE)
 */
typedef enum {
  NOT_USED = -1,
  TENTATIVE = 0,
  PREFERRED = 1,
  DEPRECATED = 2, /* not needed if we don't use pliffetime in prefix struct */
} uip_netif_state;

/**
 * \brief How the address was acquired: Autoconf, DHCP or manually
 *
 */
typedef enum {
  AUTOCONF = 1,
  DHCP = 2,
  MANUAL = 3
} uip_netif_type;

/**
 * \brief An address structure for an interface 
 *
 * Contains an ip address assigned to the interface, and its state.
 */
struct uip_netif_addr {
  uip_ipaddr_t ipaddr;
  uip_netif_state state;
  //struct timer vlifetime;
  uint8_t is_infinite;
  uip_netif_type type;
};

/** \brief  Interface structure (contains all the interface variables) */
struct uip_netif {
  uint32_t link_mtu;
  uint8_t  cur_hop_limit;
  uint32_t base_reachable_time; /* in msec */
  uint32_t reachable_time;     /* in msec */
  uint32_t retrans_timer;      /* in msec */
  uint8_t  dup_addr_detect_transmit;
  /** Note: the link-local address is at position 0 */
  struct uip_netif_addr addresses[UIP_CONF_NETIF_MAX_ADDRESSES];
  uip_ipaddr_t solicited_node_mcastaddr;
};


/*---------------------------------------------------------------------------*/
extern struct uip_netif *uip_netif_physical_if;
//extern struct etimer uip_netif_timer_dad;
//extern struct etimer uip_netif_timer_rs;
//extern struct etimer uip_netif_timer_periodic;

/*---------------------------------------------------------------------------*/
/** \brief Initialize the network interfac and run stateless autoconf */
//void  uip_netif_init(void);


/**
 * \brief Check if an unicast address is attached to my interface
 * \param ipaddr an IP address to be checked
 * \return 1 if address is attached to my interface (otherwise false)
 */
#define uip_netif_is_addr_my_unicast(a) (uip_netif_addr_lookup(a, 128, 0) != NULL)

/**
 * \brief Check if this is my solicited-node multicast address
 * \param ipaddr an IP address to be checked
 * \return 1 if the address is my solicited-node (otherwise false)
 */
//uint8_t  uip_netif_is_addr_my_solicited(uip_ipaddr_t *ipaddr);

/**
 * \brief Autoconfigure and add an address corresponding to a specific prefix
 * \param ipaddr the prefix if we are doing autoconf, the address for DHCP and manual
 * \param length the prefix length if autoconf, 0 for DHCP and manual
 * \param vlifetime valid lifetime of the address, 0 if the address has INFINITE lifetime, 
 *        non 0 otherwise
 * \param type AUTOCONF or MANUAL or DHCP 
 */
//void uip_netif_addr_add(uip_ipaddr_t *ipaddr, uint8_t length, clock_time_t vlifetime, uip_netif_type type);

/**
 * \brief Set the 8 last bytes of the IP address
 *        based on the L2 identifier using autoconf
 * \param *ipaddr the IP address to be completed with layer 2 info
 * \param *lladdr the L2 address
 */
//void uip_netif_addr_autoconf_set(uip_ipaddr_t *ipaddr, uip_lladdr_t *lladdr);

/**
 * \brief Lookup an address
 * \param ipaddr the prefix if we are looking for an autoconf address, the address otherwise
 * \param length the prefix length if we are looking for autoconf address, 128 otherwise
 * \param type AUTOCONF or MANUAL or DHCP or 0
 * 
 *
 * If we are looking for an AUTOCONFIGURED address, ipaddr is a prefix
 * length is its length, type is AUTOCONF.
 * Otherwise ipaddr is a full address, length must be 128, type is MANUAL 
 * or DHCP.
 * Note: if we do not care about the type, type MUST be 0 
 */
struct uip_netif_addr *
uip_netif_addr_lookup(uip_ipaddr_t *ipaddr, uint8_t length, uip_netif_type type);

/**
 * \brief Select the appropriate source address for a packet
 * \param ipaddr the selected address (returned value)
 * \param ipaddr the destination of the packet 
 */
void uip_netif_select_src(uip_ipaddr_t *src, uip_ipaddr_t *dst);


#endif

/** @} */

