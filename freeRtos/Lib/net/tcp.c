/**
 * @file        tcp.c
 * @author      Adam Kaliszan
 * @brief       TCP socket
 * @ingroup     network
 * @defgroup    netstack Network Stack
 * @version     0.2
 * Created:     13.10.2010
 * Revised:     05.10.2010
 * Editor Tabs: 2
 *
 */

#include "tcp.h"


static struct TcpIpSocket* findConnectedSocket(uint8_t *packet);

/**
 * @param *packet message that initializes connection
 */
static struct TcpIpSocket* tcpAcceptConn(uint8_t *packet);

void socketInit(void)
{
  sockets = xmalloc(NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
  memset(sockets, 0, NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
  
  uint8_t i;
  uint8_t *ptr = ETH_SOCK_TX_1_BUF_ADDR;
  for (i=0; i<NUMBER_OF_SOCKETS; i++)
  {
    sockets[i].Rx = xQueueCreateExternal(255, 1, (void *)(ptr));
    ptr+=256;

    sockets[i].Tx = xQueueCreateExternal(255, 1, (void *)(ptr));
    ptr+=256;

    uint8_t tmp = (uint16_t)(ptr)>>8;
    sockets[i].EncBuf.addrH          = tmp;
    sockets[i].EncBuf.ackIdx.ptr.H   = tmp;
    sockets[i].EncBuf.readIdx.ptr.H  = tmp;
    sockets[i].EncBuf.writeIdx.ptr.H = tmp;
    ptr+=256;

    if (i < 16)
      sockets[i].localPort = ((MYTELNETPOERT_H<<8) + MYTELNETPOERT_L+i);
    else
      sockets[i].localPort = ((MYTELNETPOERT_H<<8) + MYTELNETPOERT_L+16);      
  }
}

void startListen(uint8_t sockNo, uint16_t port);

struct TcpIpSocket* tcpAcceptConn(uint8_t *packet)
{
  packet = NULL;
  return NULL;
}

uint8_t processTcpPacket(uint8_t *packet)
{
  struct TcpIpSocket *socket = findConnectedSocket(packet);
  if (socket == NULL)
    socket = tcpAcceptConn(packet);
  
  if (socket == NULL)
    return 1;
  
  //Read data and put into the queue
  
  return 0;
}

struct TcpIpSocket* findConnectedSocket(uint8_t *packet)
{
  struct TcpIpSocket *result = sockets;
  uint8_t i;
  for (i=0; i<NUMBER_OF_SOCKETS; i++)
  {
    if ((result->stateFlags & CONNECTED)
      && (result->RemoteIpAddr == packet[IP_SRC_P]) 
      && (result->localPort    == htons(*((uint16_t *)(&packet[TCP_DST_PORT_H_P])))) 
      && (result->remotePort   == htons(*((uint16_t *)(&packet[TCP_SRC_PORT_H_P])))))
      return result;
    result++;
  }
  return NULL;
}

uint8_t sendTcBuffer(uint8_t socketNo)
{
  struct TcpIpSocket *sck = &sockets[socketNo];
  uint8_t space = sck->EncBuf.ackIdx.ptr.L - sck->EncBuf.writeIdx.ptr.L;
  
  if (space < 50)
    return 1;

  uint8_t i;
  uint8_t tmpPtr;
  

  uint8_t result = 0;
  uint8_t data;
  while (space > 0)
  {
    if (xQueueReceive( sck->Tx, &data, 0))
    {
      *((uint8_t *)(sck->EncBuf.writeIdx.ptr16)) = data;
      sck->EncBuf.writeIdx.ptr.L++;
      result++;
      space--;
    }
    else
      break;
  }
  
  ipSend(sck->RemoteIpAddr, IP_PROTO_TCP, result); //TODO add round buffer support

  return 0;
}

void netstackTCPIPProcess(tcpip_hdr* packet)
{
  struct netTcpHeader *tcpPacket = (struct netTcpHeader *)(&packet->tcp);
  if (tcpPacket->destport == htons(80))
  {
#if TCP_DEBUG
    if (tcpDebugStream != NULL)
      fprintf_P(tcpDebugStream, PSTR("HTTP not implemented\r\n"));
#endif
    ;
  }
  else
  {
#if TCP_DEBUG
    if (tcpDebugStream != NULL)
      fprintf_P(tcpDebugStream, PSTR("NetStack TCP/IP Rx Handler has to be implemented\r\n"));
#endif
    ;
  }
}

#if TCP_DEBUG
void setTcpDebug(FILE *stream, uint8_t level)
{
  tcpDebugStream = stream;
  tcpDebugLevel = 0;
}
#endif /* TCP_DEBUG */

void flushTcpQueues()
{
  ;
}
