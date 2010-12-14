#include "udp.h"
#include "net.h"
#include "ip.h"
#include "softwareConfig.h"

static uint32_t udpIpDst_eep   __attribute__((section (".eeprom"))) = ((uint32_t)UDP_DST_IP4   << 24) + ((uint32_t)UDP_DST_IP3   <<16) + ((uint32_t)UDP_DST_IP2   <<8) + UDP_DST_IP1;
static uint16_t udpPortDstEep  __attribute__((section (".eeprom"))) = HTONS(UDP_DST_PORT);
static uint16_t udpPortSrcEep  __attribute__((section (".eeprom"))) = HTONS(UDP_SRC_PORT);

void udpInit(void)
{
#if UDP_DEBUG
  udpDbgStream = NULL;
  udpDbgLevel = 0;
#endif

  udpSocket.dstIp   = eeprom_read_dword(&udpIpDst_eep);
//  udpSocket.dstPort = 
//  udpSocket.srcPort = 
}

#if UDP_DEBUG
void setUdpDebug(FILE *stream, uint8_t level)
{
  udpDbgStream = stream;
  udpDbgLevel = level;
}
#endif

void udpSend(uint32_t dstIp, uint16_t dstPort, uint16_t len, uint8_t* data)
{
// make pointer to UDP header
  struct netUdpHeader* udpHeader;
// move data pointer to make room for UDP header
  data -= UDP_HEADER_LEN;
  udpHeader = (struct netUdpHeader*)data;
// adjust length to add UDP header
  len += UDP_HEADER_LEN;
// fill UDP header
  udpHeader->destport = HTONS(dstPort);
  udpHeader->srcport  = HTONS(dstPort);
  udpHeader->udplen = htons(len);
  udpHeader->udpchksum = 0;

#if UDP_DEBUG
  if (udpDbgStream != NULL)
    fprintf_P(udpDbgStream, PSTR("debugPrintHexTable(UDP_HEADER_LEN, (uint8_t*)udpPacket)"));
#endif

  ipSend(dstIp, IP_PROTO_UDP, len);
}

void netstackUDPIPProcess(unsigned int len, udpip_hdr* packet)
{
  uint8_t len = (uint8_t) htons(packet->udp.udplen);
  uint8_t i;

  #if UDP_DEBUG
  if(udpDbgStream != NULL)
    if (udpDbgLevel > 3)
      fprintf_P(udpDbgStream, PSTR("Proc UDP packet len %d\r\n"), len);
#endif

  if (packet->udp.destport == htons(3000))
  {
    uint8_t *tmp = (uint8_t *)(&packet->udp.udpchksum) + 2;
    for (i=UDP_HEADER_LEN; i<len; i++)
    {
      xQueueSend(xVtyRec, tmp, 10);
#if UDP_DEBUG
      if(udpDbgStream != NULL)
        if (udpDbgLevel > 2)
          fprintf_P(udpDbgStream, PSTR("rec 0x%2x\r\n"), *tmp);
#endif
      tmp++;
    }
  }
}
