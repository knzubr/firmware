/**
 * @file        Enc28j60socket.c
 * @author      Adam Kaliszan
 * @brief       Enc28j60 socket
 * @version     0.1
 * Created:     10/13/2010
 * Revised:     10/13/2010
 * Editor Tabs: 2
 *
 */
//@{
#include <avr/io.h>
#include "socket.h"
#include "memory_x.h"

//@{


void socketInit(void)
{
  sockets = xmalloc(NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
  memset(sockets, 0, NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
}

void startListen(uint8_t sockNo, uint16_t port);
void processIpPacket(char *packet);


struct TcpIpSocket* findConnectedSocket(uint32_t dstIp, uint16_t srcPort, uint16_t dstPort)
{
  TcpIpSocket *result = sockets;
  uint8_t i;
  for (i=0; i<NUMBER_OF_SOCKETS; i++)
  {
    if ((result->stateFlags & CONNECTED) && (result->dstIpAddr == dstIp) 
      && (result->dstPort == dstPort) && (result->srcPort == srcPort))
      return result;
    result++;
  }
  return NULL;
}
//@}