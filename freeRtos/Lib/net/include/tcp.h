/**
 * @file        tcp.h
 * @author      Adam Kaliszan
 * @brief       Enc28j60 TCP socket 
 * @ingroup     network
 * @version     0.1
 * Created:     13.10.2010
 * Revised:     .12.2010
 * Editor Tabs: 2
 *
 * @defgroup    netstack Network Stack
 * @code #include "net/Enc28j60socket.h" @endcode
 * @par Description
 *  This library supports TCP Sockets. 
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

#include <stdio.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <string.h>
#include <avr/io.h>

#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "memory_x.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

#include "net.h"
#include "ip.h"

#if TCP_DEBUG
FILE *tcpDebugStream;
uint8_t tcpDebugLevel;

/**
 * Enables TCP protocol debuging
 * @param *stream - pointer to output debug stream
 * @param level   - debug intensity 0 - debug sidabled, 1 - basic short info, 2 - short info about all ewents, 3 - basic detailed info + short info about all ewents, 4 - detailed info about everything
 */
void setTcpDebug(FILE *stream, uint8_t level);
#endif

enum socketState
{
  INNACTIVE,
  LISTEN,
  CONNECTED
};

#define CONNECTED 0x01
#define LISTEN    0x02
#define DO_ACK    0x04

typedef union 
{
  void *ptr16;
  struct
  {
    uint8_t L;
    uint8_t H;
  } ptr;
} brutalPtr_t;

typedef struct
{
  brutalPtr_t  readIdx;
  brutalPtr_t  writeIdx;
  brutalPtr_t  ackIdx; 
  uint8_t      addrH;
} roundBuffer;

typedef struct
{
  uint8_t sequence;
  uint8_t memAddrH;
} outOfSeq;

struct TcpIpSocket
{
  uint8_t      RemoteMAC[6];           /// Remote Mac Address. We don't use ARP
  uint32_t     RemoteIpAddr;           ///
  uint16_t     localPort;              /// Stored in hest order
  uint16_t     remotePort;             /// Stored in host order 
  
  uint32_t     seqNoLastReceived;      /// Sequence number of last received packet
  uint16_t     noOfNotAckBytes;        /// Number of received bytes without ack
  
  uint16_t     windowSize;
  
  uint8_t      encBufferAddrHi;        /// Enc Buf address bits (12-8)
  uint8_t      encBufferWrAddrLo;      /// Enc buffer address for writing
  
  uint8_t      droped;                 /// Some packets ware droped, because they came out of sequence and theare was no room in memory to store it
  outOfSeq     recBuffer[4];           /// Buffer with packets, that came out of sequence 
  
  xQueueHandle Rx;
  xQueueHandle Tx;
  
  // Frame buffer size is 256 bytes
  roundBuffer  EncBuf;
  
  //struct packetBackup packetBackupBuffer[4];
  uint8_t  packetBackupBufferNotConfirmedPacketIdx;
  uint8_t  packetBackupBufferWriteIdx;
  
  uint8_t stateFlags;
};

struct TcpIpSocket *sockets;



/**
 * Initialize socket structs
 */
void socketInit(void);



/**
 * put received data into the matching socket,
 * if theare is matching sockets, creates new connection
 * @param packet - IP packet
 * @return 0 - OK, 1 - socket is ocupied by another connection
 */
uint8_t processTcpPacket(uint8_t *packet);

/**
 * Reads number of bytes that are queued in Tx buffer
 */
uint8_t getTxBufferState(uint8_t socketNo);


/**
 * Sends data from Tx buffer
 * @param socketNo - socket number
 * @return 0 - all OK
 */
uint8_t sendTcBuffer(uint8_t socketNo);

/**
 * Close TCP socket
 * @param socketNo - socket number to be closed
 * @return 0 - all OK
 */
uint8_t closeSocket(uint8_t socketNo);


void netstackTCPIPProcess(tcpip_hdr* packet);

/**
 * Flush all UDP queues
 */
void flushTcpQueues(void);


#endif /*SOCKET_H*/
//@}
