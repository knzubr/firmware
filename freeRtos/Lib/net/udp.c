#include "udp.h"
#include "net.h"
#include "ip.h"

static uint32_t udpIpDst_eep   __attribute__((section (".eeprom"))) = ((uint32_t)UDP_DST_IP4   << 24) + ((uint32_t)UDP_DST_IP3   <<16) + ((uint32_t)UDP_DST_IP2   <<8) + UDP_DST_IP1;
static uint16_t udpPortDstEep  __attribute__((section (".eeprom"))) = HTONS(UDP_DST_PORT);
static uint16_t udpPortSrcEep  __attribute__((section (".eeprom"))) = HTONS(UDP_SRC_PORT);

void udpInit(void)
{
#if UDP_DEBUG
  udpDbgStream         = NULL;
  udpDbgLevel          = 0;
#endif

  udpSocket.dstIp      = eeprom_read_dword(&udpIpDst_eep);
  udpSocket.dstPortDef = eeprom_read_word(&udpPortDstEep);;
  udpSocket.srcPort    = eeprom_read_word(&udpPortSrcEep);
  
  udpSocket.Rx         = xQueueCreateExternal(255, 1, (void *)(RTOS_UDP_RX_BUF_ADDR));
  udpSocket.Tx         = xQueueCreateExternal(255, 1, (void *)(RTOS_UDP_TX_BUF_ADDR));
}

#if UDP_DEBUG
void setUdpDebug(FILE *stream, uint8_t level)
{
  udpDbgStream = stream;
  udpDbgLevel = level;
}
#endif

inline void udpSend(uint16_t len)
{
// make pointer to UDP header
  struct netUdpHeader* udpHeader = (struct netUdpHeader*)(&nicState.buf[ETH_HEADER_LEN + IP_HEADER_LEN]);

  udpHeader->srcport  = udpSocket.srcPort;
  udpHeader->destport = (udpSocket.dstPortDef == 0)? udpSocket.dstPort : udpSocket.dstPortDef;          //data in udpSocket are stored in network order
  
  udpHeader->udplen = htons(len + UDP_HEADER_LEN);
  udpHeader->udpchksum = 0;

#if UDP_DEBUG
  if (udpDbgStream != NULL)
    if (udpDbgLevel > 1)
      fprintf_P(udpDbgStream, PSTR("Sending UDP packet (data length %d)\r\n"), len);
#endif
  ipSend(udpSocket.dstIp, IP_PROTO_UDP, len + UDP_HEADER_LEN);
}

inline void netstackUDPIPProcess(void)
{
  struct netUdpHeader* udpHeader = (struct netUdpHeader*)(&nicState.buf[ETH_HEADER_LEN + IP_HEADER_LEN]);
  uint16_t len = (uint16_t) htons(udpHeader->udplen);
  uint8_t i;

  #if UDP_DEBUG
  if(udpDbgStream != NULL)
    if (udpDbgLevel > 3)
      fprintf_P(udpDbgStream, PSTR("Proc. UDP packet (data length %d)\r\n"), len-UDP_HEADER_LEN);
#endif

  if ((udpHeader->destport == udpSocket.srcPort) && ((udpSocket.dstPortDef == HTONS(0)) || (udpSocket.dstPort == udpHeader->srcport)))
  {
    if (udpSocket.dstPortDef == HTONS(0))
      udpSocket.dstPort = udpHeader->srcport;
    uint8_t *tmp = (uint8_t *)(&nicState.buf[ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN]);
#if UDP_DEBUG
      if(udpDbgStream != NULL)
        if (udpDbgLevel > 4)
          fprintf_P(udpDbgStream, PSTR("Received data: "));
#endif

    for (i=UDP_HEADER_LEN; i<len; i++)
    {
#if UDP_DEBUG
      if(udpDbgStream != NULL)
        if (udpDbgLevel > 4)
          fprintf_P(udpDbgStream, PSTR("0x%2x\r\n"), *tmp);
#endif
#if UDB_DEBUG
      xQueueSend(udpSocket.Rx, tmp, 10) == 0);
#else
      if (xQueueSend(udpSocket.Rx, tmp, 10) == 0)
        if(udpDbgStream != NULL)
          if (udpDbgLevel > 0)
            fprintf_P(udpDbgStream, PSTR("UDP TX buffer busy\r\n"));
#endif
      tmp++;
    }
#if UDP_DEBUG
    if(udpDbgStream != NULL)
      if (udpDbgLevel > 4)
        fprintf_P(udpDbgStream, PSTR("\r\n"));
#endif

  }
}

inline void flushUdpQueues(void)
{
  if (uxQueueMessagesWaiting(udpSocket.Tx) > 0)
  {
    uint16_t len = 0;
    uint8_t *data = &nicState.buf[ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN];
    while (xQueueReceive(udpSocket.Tx, data, 0) == pdTRUE)
    {
      data++;
      len++;
    }
    udpSend(len);
  }
}

void udpSaveConfig(void)
{
  eeprom_update_dword(&udpIpDst_eep, udpSocket.dstIp);
  eeprom_update_word(&udpPortDstEep, udpSocket.dstPortDef);
  eeprom_update_word(&udpPortSrcEep, udpSocket.srcPort);
}

void udpPrintStatus(FILE *stream)
{
  fprintf_P(stream, PSTR("UDP config:"));
  fprintf_P(stream, PSTR("\r\n  IP          : ")); netPrintIPAddr(stream, udpSocket.dstIp);
  fprintf_P(stream, PSTR("\r\n  src port    : %d\r\n  dst port    : "), htons(udpSocket.srcPort));
  if (udpSocket.dstPortDef == HTONS(0))
    fprintf_P(stream, PSTR("ANY\r\n"));
  else
    fprintf_P(stream, PSTR("%d\r\n"), htons(udpSocket.dstPortDef));    
}
