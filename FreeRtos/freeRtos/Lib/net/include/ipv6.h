#ifndef IPV6_H
#define IPV6_H 1

/**
 * Definicja zmiennych potrzebnych do obsługi IPv6
 * (część deklaracji może się znajdować w pliku net.h i nic.h)
 * 
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "net.h"
#include "nic.h"
#include "softwareConfig.h"
#include "ipv6Conf.h"

#if IPV6_SUPPORT

/** Kompatybilność ze stosem Adama Kaliszana*/
#define UIP_PROTO_ICMP  IP_PROTO_ICMP //1
#define UIP_PROTO_TCP   IP_PROTO_TCP //6
#define UIP_PROTO_UDP   IP_PROTO_UDP //17
#define UIP_PROTO_ICMP6 58


/**
 * IP addressing/configuration structure
 */
struct ipv6Config                 
{
  uip_ip6addr_t *ip;                  ///< IPv6 address
  uint8_t       prefix;               ///< netmask (prefix)
  uip_ip6addr_t *gateway;             ///< gateway IPv6 address
};

struct ipv6Config   Ipv6MyConfig;     ///< Local IP address/config structure
extern nicState_t nicState;

#define PRINTF(...) fprintf_P(__VA_ARGS__)
#define PRINT6ADDR(stream, addr) PRINTF(stream, PSTR("IPv6 addr: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x \r\n"),((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])


/**
 * Process Ip v6 packet
 */
void netstackIPv6Process(void);

/**
 * Init an IPv6 packet.
 */
void ipv6Init(void);

/**
 * @param myIp   - ipv6 Address
 */
void ipSetConfigIpv6(uint32_t myIp);

/**
 * Get our local IPv6 configuration.
 * @return pointer to current IPv6 address/configuration.
 */
//struct ipv6Config* ipv6GetConfig(void);

/**
 * Print IPv6 configuration
 * @param straem - input stream
 * @param *config - pointer to IPv6 config struct
 */
void ipv6PrintConfig(FILE *stream, struct ipv6Config* config);

/**
 * Send an IPv6 packet.
 */
//void ipv6Send(uint32_taa dstIpv6, uint8_t protocol, uint16_t len);

/**
 * Print IPv6 header information.
 */
void netPrintIpv6Header(FILE *stream);

/**
 * Print IPv6 header information. (Raw bits - hex format)
 */
void netPrintIpv6HeaderRAW(FILE *stream);

#if IP_DEBUG
/**
 * Init debug stram
 */
void ipv6DebugInit(FILE *inDebugStream);
FILE *debugStream;
#endif

#endif /* IPV6_SUPPORT */
#endif /*IPV6_H*/