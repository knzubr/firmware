/**
 * @file        Enc28j60socket.h
 * @author      Adam Kaliszan
 * @brief       Enc28j60 socket
 * @version     0.1
 * Created:     10/13/2010
 * Revised:     10/13/2010
 * Editor Tabs: 2
 *
 * @ingroup     network
 * @defgroup    netstack Network Stack
 * @code #include "net/Enc28j60socket.h" @endcode
 * @par Description
 *  This library supports TCP Sockets. 
 *  Layer 4 (TCP) buffer placed in enc28j60 chip, so this library works only with Enc28j60 chip.
 *  There is no network layer model. It allows to save microcontroller memory.
 *  
 * @note This is NOT a full-blown TCP/IP stack. It merely handles lower
 *  level stack functions so that TCP packets can be sent and received easily.
 *  End-to-end TCP functionality may be added in a future version.
 *  Until then, I can recommend using other embedded TCP/IP stacks like Adam Dunkel's uIP.
 *
 */
//@{


#ifndef SOCKET_H
#define SOCKET_H

#define NUMBER_OF_SOCKETS 20


#include <inttypes.h>
#include "main.h"
#include "hardwareConfig.h"
#include "memory_x.h"

enum socketState
{
  INNACTIVE,
  LISTEN,
  CONNECTED
};

struct packetBackup
{
  uint8_t seqNoLo;                 /// Sequence number bits (7-0)
  uint8_t addressLo;               /// ENC Buf address bits (7-0)
};

#define CONNECTED 0x01
#define LISTEN    0x02
#define DO_ACK    0x04

struct TcpIpSocket
{
  uint32_t dstIpAddr;
  uint16_t dstPort;
  uint16_t srcPort;
  
  uint32_t seqNo;

  uint32_t seqNoToAck;             /// Sequence number to Acknowledge
  uint32_t seqNoLastReceived;      /// Sequence number of last received packet
  uint16_t noOfNotAckBytes;        /// Number of received bytes without ack
  
  uint16_t windowSize;
  
  uint8_t  encBufferAddrHi;        /// Enc Buf address bits (12-8)
  uint8_t  encBufferWrAddrLo;      /// Enc buffer address for writing
  
  //TODO Free Rtos Rx que
  //TODO Free Rtos Tx que
  

  struct packetBackup packetBackupBuffer[4];
  uint8_t  packetBackupBufferNotConfirmedPacketIdx;
  uint8_t  packetBackupBufferWriteIdx;
  
  uint8_t stateFlags;
};

/**
 * Initialize socket structs
 */
void socketInit(void);

void startListen(uint8_t sockNo, uint16_t port);
void processIpPacket(char *packet);

/**
 * Find socket struct that matches to received TCP packet
 * @param dstIp   - destination IP (source IP of received packet)
 * @param srcPort - source port (destination port of received packet)
 * @param dstPort - destination port (source port of received packet)
 * @return pointer to matching socket struct or NULL
 */
struct TcpIpSocket* findConnectedSocket(uint32_t dstIp, uint16_t srcPort, uint16_t dstPort);


struct TcpIpSocket *sockets;

#endif SOCKET_H
//@}
