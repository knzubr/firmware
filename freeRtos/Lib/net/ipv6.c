/**
 * Deklaracje glownych funkcji ipv6
 */

#include "ipv6.h"
#include "../../freeRtos/Lib/net/include/ipv6.h"

void ipv6Init(void)
{
  Ipv6MyConfig.ip = xmalloc(sizeof(uip_ip6addr_t));
  Ipv6MyConfig.prefix= 64;
  Ipv6MyConfig.gateway = xmalloc(sizeof(uip_ip6addr_t));
}

void netstackIPv6Process(void)
{
  //uip_ip6addr_t *ipv6addr;
  fprintf_P(debugStream, PSTR("IPv6 packet in.\r\n"));
    

  netPrintIpv6Header(debugStream);
  
}

void ipv6DebugInit(FILE* inDebugStream)
{
  debugStream = inDebugStream;
}


void netPrintIpv6HeaderRAW(FILE *stream)
{
  fprintf_P(stream, PSTR("IPv6 Header\r\n"));    
  for (uint8_t i=0; i < UIP_IPv6H_LEN; i++)
  {
    if (i!=0 && i%16==0) fprintf_P(stream, PSTR("\r\n"));
    fprintf_P(stream, PSTR("%02x:"), nicState.layer3.buf[i]);
  }
  fprintf_P(stream, PSTR("\r\n")); 
}

void netPrintIpv6Header(FILE *stream)
{
    fprintf_P(stream, PSTR("Version: \t\t"));
    fprintf_P(stream, PSTR("%d \n\r"), nicState.layer3.ipv6->vtc >> 4);
    fprintf_P(stream, PSTR("Traffic Class: \t\t"));
    fprintf_P(stream, PSTR("%d \n\r"), nicState.layer3.ipv6->vtc & 0x0F + nicState.layer3.ipv6->tcflow >> 4);
    fprintf_P(stream, PSTR("Flow Label: \t\t"));
    fprintf_P(stream, PSTR("%d \n\r"), nicState.layer3.ipv6->tcflow & 0x0F + nicState.layer3.ipv6->flow << 4);
    fprintf_P(stream, PSTR("Payload Length: \t"));
    fprintf_P(stream, PSTR("%d \n\r"), nicState.layer3.ipv6->len[1] + nicState.layer3.ipv6->len[0]);
    fprintf_P(stream, PSTR("Source address: \t"));
    PRINT6ADDR(stream, &nicState.layer3.ipv6->srcipaddr);
    fprintf_P(stream, PSTR("Destination address: \t"));
    PRINT6ADDR(stream, &nicState.layer3.ipv6->srcipaddr);
}


//void ipv6Send(uint32_t dstIpv6, uint8_t protocol, uint16_t len);
//void netPrintIpv6Header(FILE *stream, struct netIpv6Header* ipheader);
//void ipSetConfigIpv6(uint32_t myIp);
//void ipv6PrintConfig(FILE *stream, struct ipv6Config* config);