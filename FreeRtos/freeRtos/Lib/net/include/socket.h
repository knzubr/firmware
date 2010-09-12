/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : Socket API
* Author     : Adam Kaliszan
* Copyright: GPL V2
*
*****************************************************************************/
//@{


#ifndef SOCKET_H
#define SOCKET_H

#include <inttypes.h>
#include "main.h"
#include "hardwareConfig.h"

enum socketState
{
  INNACTIVE,
  LISTEN,
  CONNECTED
};

struct TcpIpSocket
{
  uint32_t dstIpAddr;
  uint16_t dstPort;
  uint16_t srcPort;
  
  uint32_t seqNo;
  uint16_t windowSize;
  uint16_t noOfNotAckBytes;
  
  enum socketState state;
};


void socketInit(void);
void startListen(uint8_t sockNo, uint16_t port);
void startListenIp(uint8_t sockNo, uint16_t port, uint32_t ipAddr);

void processIpPacket(char *packet);

#endif SOCKET_H
//@}
