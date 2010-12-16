/*! \file icmp.c \brief ICMP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'icmp.c'
// Title		: ICMP (Internet Control Message Protocol) Protocol Library
// Author		: Pascal Stang
// Created		: 9/10/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************


#include "icmp.h"


//extern void nicSend(unsigned int len, unsigned char* packet);

// global variables


// functions
void icmpInit(void)
{
  icmpDebug = NULL;
  icmpDebugLevel = 0;
}

void icmpIpIn(icmpip_hdr* packet)
{
  // check ICMP type
  switch(packet->icmp.type)
  {
    case ICMP_TYPE_ECHOREQUEST:
      icmpEchoRequest(packet);       // echo request
      break;
    default:
#if ICMP_DEBUG
      if (icmpDebug != NULL)
      {
        if (icmpDebugLevel > 0)
          fprintf_P(icmpDebug, PSTR("Unknown ICMP typeReceived ICMP request: "));
      }
#endif      
      break;
  }
}

void icmpEchoRequest(icmpip_hdr* packet)
{
#if ICMP_DEBUG
  if (icmpDebug != NULL)
  {
    if (icmpDebugLevel > 1)
      fprintf_P(icmpDebug, PSTR("Received ICMP request: "));
  }
#endif
  uint32_t tempIp;

  // change type to reply
  packet->icmp.type = ICMP_TYPE_ECHOREPLY;
  // recalculate checksum
  packet->icmp.icmpchksum = 0;
  packet->icmp.icmpchksum = netChecksum((uint8_t*)(&packet->icmp), htons(packet->ip.len)-IP_HEADER_LEN);
  // return to sender
  tempIp = packet->ip.destipaddr;
  packet->ip.destipaddr = packet->ip.srcipaddr;
  packet->ip.srcipaddr = tempIp;
  // add ethernet routing
  arpIpOut((struct netEthIpHeader*)(((uint8_t*)(packet))-ETH_HEADER_LEN), 0);

  // debugging
  if (icmpDebug != NULL)
    icmpPrintHeader(icmpDebug, packet);
  //debugPrintHexTable(htons(packet->ip.len), (uint8_t*)packet);

  // send it (packet->ip.len+ETH_HEADER_LEN
  nicSend(htons(packet->ip.len)+ETH_HEADER_LEN);
#if ICMP_DEBUG
  if (icmpDebug != NULL)
  {
    if (icmpDebugLevel > 1)
      fprintf_P(icmpDebug, PSTR("Sending ICMP PONG\r\n"));
  }
#endif
}

#if ICMP_DEBUG
void setIcmpDebug(FILE *stream, uint8_t level)
{
  icmpDebug = stream;
  icmpDebugLevel = level;
}
#endif

void icmpPrintHeader(FILE *stream, icmpip_hdr* packet)
{
  fprintf_P(stream, PSTR("ICMP Packet:\r\n"));
// print source IP address
  fprintf_P(stream, PSTR("SrcIpAddr: "));  netPrintIPAddr(stream, packet->ip.srcipaddr);  fprintf_P(stream, PSTR("\r\n"));
// print dest IP address
  fprintf_P(stream, PSTR("DstIpAddr: "));  netPrintIPAddr(stream, packet->ip.destipaddr); fprintf_P(stream, PSTR("\r\n"));
// print type
  fprintf_P(stream, PSTR("Type   : "));
  switch(packet->icmp.type)
  {
    case ICMP_TYPE_ECHOREQUEST:
      fprintf_P(stream, PSTR("ECHO REQUEST"));
      break;
    case ICMP_TYPE_ECHOREPLY:
      fprintf_P(stream, PSTR("ECHO REPLY"));
      break;
    default:
      fprintf_P(stream, ("UNKNOWN"));
      break;
  }
  fprintf_P(stream, PSTR("\r\n"));
// print code
  fprintf_P(stream, PSTR("Code   : 0x%x\r\n"), packet->icmp.icode);
}

