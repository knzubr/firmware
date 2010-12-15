/**
 * @file ip.c 
 * @brief IP (Internet Protocol) Library. 
 */
//*****************************************************************************
//
// File Name	: 'ip.c'
// Title		: IP (Internet Protocol) Library
// Author		: Pascal Stang
// Created		: 8/30/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 2
//
//*****************************************************************************

#include "ip.h"

static uint32_t myip_eep    __attribute__((section (".eeprom"))) = ((uint32_t)MY_IP4   << 24) + ((uint32_t)MY_IP3   <<16) + ((uint32_t)MY_IP2   <<8) + MY_IP1;
static uint32_t mask_eep    __attribute__((section (".eeprom"))) = ((uint32_t)MY_MASK4 << 24) + ((uint32_t)MY_MASK3 <<16) + ((uint32_t)MY_MASK2 <<8) + MY_MASK1;
static uint32_t defGw_eep   __attribute__((section (".eeprom"))) = ((uint32_t)MY_GW4   << 24) + ((uint32_t)MY_GW3   <<16) + ((uint32_t)MY_GW2   <<8) + MY_GW1;

void ipInit(void)
{
  IpMyConfig.ip      = eeprom_read_dword(&myip_eep);
  IpMyConfig.netmask = eeprom_read_dword(&mask_eep);
  IpMyConfig.gateway = eeprom_read_dword(&defGw_eep);
#if IP_DEBUG
  IpMyConfig.dbgLevel = 0;
  IpMyConfig.dbgStream = NULL;
#endif
}

void ipSaveConfig(void)
{
  eeprom_update_dword(&myip_eep,  IpMyConfig.ip);
  eeprom_update_dword(&mask_eep,  IpMyConfig.netmask);
  eeprom_update_dword(&defGw_eep, IpMyConfig.gateway);
}

void netstackIPProcess(ip_hdr* packet)
{
// check IP addressing, stop processing if not for me and not a broadcast
  if( (packet->destipaddr != ipGetConfig()->ip) &&
      (packet->destipaddr != (ipGetConfig()->ip|ipGetConfig()->netmask)) &&
      (packet->destipaddr != 0xFFFFFFFF)) 
    return;

// handle ICMP packet
  if( packet->proto == IP_PROTO_ICMP )
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: ICMP/IP packet\r\n"));
      if (IpMyConfig.dbgLevel > 2)
        icmpPrintHeader(IpMyConfig.dbgStream, (icmpip_hdr*)packet);
    }
#endif /*IP_DEBUG*/
    icmpIpIn((icmpip_hdr*)packet);
  }
  else if( packet->proto == IP_PROTO_UDP )
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: UDP/IP packet\r\n"));
    }
#endif /*IP_DEBUG*/
    netstackUDPIPProcess(((udpip_hdr*)packet) );
  }
  else if( packet->proto == IP_PROTO_TCP )
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: TCP/IP packet\r\n"));
    }
#endif /*IP_DEBUG*/
    netstackTCPIPProcess(((tcpip_hdr*)packet) );
  }
  else
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: IP packet\r\n"));
    }
#endif /*IP_DEBUG*/
  }
}

#if IP_DEBUG
void setIpDebug(FILE *stream, uint8_t level)
{
  IpMyConfig.dbgStream = stream;
  IpMyConfig.dbgLevel = level;
  if (level == 0)
    IpMyConfig.dbgStream = NULL;
}
#endif

void ipSetConfig(uint32_t myIp, uint32_t netmask, uint32_t gatewayIp)
{
  // set local addressing
  IpMyConfig.ip = myIp;
  IpMyConfig.netmask = netmask;
  IpMyConfig.gateway = gatewayIp;
}

void ipSetConfigIp(uint32_t myIp)
{
  // set local addressing
  IpMyConfig.ip = myIp; 
}

void ipSetConfigMask(uint32_t netmask)
{
  IpMyConfig.netmask = netmask;
}

void ipSetConfigGw(uint32_t gatewayIp)
{
  IpMyConfig.gateway = gatewayIp;
}


struct ipConfig* ipGetConfig(void)
{
  return &IpMyConfig;
}

void ipSend(uint32_t dstIp, uint8_t protocol, uint16_t len)
{
// make pointer to ethernet/IP header
  struct netEthIpHeader* ethIpHeader;

  ethIpHeader = (struct netEthIpHeader*) nicState.buf;

#if IP_DEBUG
  if (IpMyConfig.dbgStream != NULL)
  {  
    if (IpMyConfig.dbgLevel > 2)
      fprintf_P(IpMyConfig.dbgStream, "debugPrintHexTable(len+ETH_HEADER_LEN+IP_HEADER_LEN, data)");
  }
#endif

// adjust length to add IP header
  len += IP_HEADER_LEN;

// fill IP header
  ethIpHeader->ip.destipaddr = dstIp;
  ethIpHeader->ip.srcipaddr = IpMyConfig.ip;
  ethIpHeader->ip.proto = protocol;
  ethIpHeader->ip.len = htons(len);
  ethIpHeader->ip.vhl = 0x45;
  ethIpHeader->ip.tos = 0;
  ethIpHeader->ip.ipid = 0;
  ethIpHeader->ip.ipoffset = 0;
  ethIpHeader->ip.ttl = IP_TIME_TO_LIVE;
  ethIpHeader->ip.ipchksum = 0;

// calculate and apply IP checksum
// DO THIS ONLY AFTER ALL CHANGES HAVE BEEN MADE TO IP HEADER
  ethIpHeader->ip.ipchksum = netChecksum((uint8_t *)(&ethIpHeader->ip), IP_HEADER_LEN);

// add ethernet routing
// check if we need to send to gateway
  if( (dstIp & IpMyConfig.netmask) == (IpMyConfig.ip & IpMyConfig.netmask) )
  {
    arpIpOut(ethIpHeader,0);  // local send
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
      fprintf_P(IpMyConfig.dbgStream, PSTR("Sending IP packet on local net\r\n"));
#endif
  }
  else
  {
    arpIpOut(ethIpHeader,IpMyConfig.gateway);  // gateway send
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
      fprintf_P(IpMyConfig.dbgStream, PSTR("Sending IP packet to gateway\r\n"));
#endif
  }

// adjust length to add ethernet header
  len += ETH_HEADER_LEN;

#if IP_DEBUG
  if (IpMyConfig.dbgStream != NULL)
  {
    fprintf_P(IpMyConfig.dbgStream, PSTR("debugPrintHexTable(ETH_HEADER_LEN, &data[0]);"));
    fprintf_P(IpMyConfig.dbgStream, PSTR("debugPrintHexTable(len-ETH_HEADER_LEN, &data[ETH_HEADER_LEN]);"));
  }
#endif
// send it
  nicSend(len);
}

void ipPrintConfig(FILE *stream, struct ipConfig* config)
{
  fprintf_P(stream, PSTR("IP Addr : ")); netPrintIPAddr(stream, config->ip);        fprintf_P(stream, PSTR("\r\n"));
  fprintf_P(stream, PSTR("Netmask : ")); netPrintIPAddr(stream, config->netmask);   fprintf_P(stream, PSTR("\r\n"));
  fprintf_P(stream, PSTR("Gateway : ")); netPrintIPAddr(stream, config->gateway);   fprintf_P(stream, PSTR("\r\n"));
}
