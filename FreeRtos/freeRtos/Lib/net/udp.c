#include "udp.h"

void udpInit(void)
{
#if UDP_DEBUG
  udpDbgStream = NULL;
  udpDbgLevel = 0;
#endif
}

#if UDP_DEBUG
void setUdpDebug(FILE *stream, uint8_t level)
{
  udpDbgStream = stream;
  udpDbgLevel = 0;
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
#if UDP_DEBUG
  if(udpDbgStream != NULL)
    fprintf_P(udpDbgStream, PSTR("Proc UDP packet (length %d)\r\n"), len);
#endif
}
