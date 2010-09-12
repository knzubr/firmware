/**
 * @file        netstack.c
 * @author      Pascal Stang, Adam Kaliszan
 * @brief       Network Stack
 * @version     0.31
 * Created:     6/28/2005
 * Revised:     9/20/2005
 * Editor Tabs: 2
 */

#include <stdio.h>
#include "netstack.h"

#include "hardwareConfig.h"

unsigned char NetBuffer[] = (unsigned char*)(ENC28J60BUF_ADDR); /// Temporary buffer for processing and creating packets

#ifdef NETSTACK_DEBUG
FILE *debugStr;                                                 /// Debug stream
#endif

#ifndef NETSTACK_DEBUG

void netstackInit(uint32_t ipaddress, uint32_t netmask, uint32_t gatewayip)
#else
void netstackInit(uint32_t ipaddress, uint32_t netmask, uint32_t gatewayip, FILE *debug)
#endif
{
  // init network device driver
  #ifdef NETSTACK_DEBUG
  debugStr = debug;
  fprintf(debugStr, "Initializing Network Device\r\n");
  #endif
//  nicInit();
  // init ARP
  #ifdef NETSTACK_DEBUG
  fprintf(debugStr, "Initializing ARP cache\r\n");
  #endif
  arpInit();
  // init addressing
  #ifdef NETSTACK_DEBUG
  fprintf(debugStr, "Initializing Addressing\r\n");
  #endif
  ipSetConfig(ipaddress, netmask, gatewayip);
}

uint8_t* netstackGetBuffer(void)
{
  return NetBuffer;
}

int netstackService(void)
{
  int len;
  struct netEthHeader* ethPacket;

// look for a packet
  len = nicPoll(NETWORK_STACK_BUF_SIZE, NetBuffer);

  if(len)
  {
    ethPacket = (struct netEthHeader*)&NetBuffer[0];

#if NET_DEBUG >= 5
    fprintf(debugStr, "Received packet len: %d, type: %d\r\n", len, htons(ethPacket->type));
    fprintf(debugStr, "Packet Contents\r\n");
    debugPrintHexTable(len, NetBuffer);
#endif

    if(ethPacket->type == htons(ETHTYPE_IP))
    {
// process an IP packet
#ifdef NETSTACK_DEBUG
      fprintf_P(debugStr, "NET Rx: IP packet\r\n");
#endif
// add the source to the ARP cache
// also correctly set the ethernet packet length before processing?
      arpIpIn((struct netEthIpHeader*)(&NetBuffer[0]));
//arpPrintTable();
      netstackIPProcess(len-ETH_HEADER_LEN, (ip_hdr*)(&NetBuffer[ETH_HEADER_LEN]) );
    }
    else if(ethPacket->type == htons(ETHTYPE_ARP))
    {
// process an ARP packet
#ifdef NETSTACK_DEBUG
      fprintf_P("NET Rx: ARP packet\r\n");
#endif
      arpArpIn(len, ((struct netEthArpHeader*)(&NetBuffer[0])) );
    }
  }
  return len;
}

void netstackIPProcess(unsigned int len, ip_hdr* packet)
{
// check IP addressing, stop processing if not for me and not a broadcast
  if ( (htonl(packet->destipaddr) != ipGetConfig()->ip) && (htonl(packet->destipaddr) != (ipGetConfig()->ip|ipGetConfig()->netmask)) && (htonl(packet->destipaddr) != 0xFFFFFFFF) ) 
    return;

// handle ICMP packet
  if( packet->proto == IP_PROTO_ICMP )
  {
#ifdef NETSTACK_DEBUG
    fprintf_P(debugStr, "NET Rx: ICMP/IP packet\r\n");
//icmpPrintHeader((icmpip_hdr*)packet);
#endif
    icmpIpIn((icmpip_hdr*)packet);
  }
  else if( packet->proto == IP_PROTO_UDP )
  {
#ifdef NETSTACK_DEBUG
    fprintf_P(debugStr, "NET Rx: UDP/IP packet\r\n");
//debugPrintHexTable(NetBufferLen-14, &NetBuffer[14]);
#endif
    netstackUDPIPProcess(len, ((udpip_hdr*)packet) );
  }
  else if( packet->proto == IP_PROTO_TCP )
  {
#ifdef NETSTACK_DEBUG
    fprintf_P(debugStr, "NET Rx: TCP/IP packet\r\n");
#endif
    netstackTCPIPProcess(len, ((tcpip_hdr*)packet) );
  }
#ifdef NETSTACK_DEBUG
  else
  {
    fprintf_P(debugStr, "NET Rx: IP packet\r\n");
  }
#endif
}

void netstackUDPIPProcess(unsigned int len, udpip_hdr* packet)
{
#ifdef NETSTACK_DEBUG
  fprintf(debugStr, "NetStack UDP/IP Rx Dummy Handler\r\n");
#endif
}

void netstackTCPIPProcess(unsigned int len, tcpip_hdr* packet)
{
#ifdef NETSTACK_DEBUG
  fprintf(debugStr, "NetStack TCP/IP Rx Dummy Handler\r\n");
#endif
}
