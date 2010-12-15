#ifndef UDP_H
#define UDP_H

#include <avr/pgmspace.h>
#include "softwareConfig.h"
#include "ip.h"
#include "main.h"
#include "avr/eeprom.h"

#if UDP_DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

extern struct ipConfig IpMyConfig;


typedef struct
{
  uint16_t dstPort;             //stored in network order
  uint16_t srcPort;
  uint32_t dstIp;  
  
  xQueueHandle Rx;
  xQueueHandle Tx;
} UdpSocket_t;

extern xQueueHandle xVtyRec;

UdpSocket_t udpSocket[NUMBER_OF_UDP_SOCK];

#if UDP_DEBUG
FILE *udpDbgStream;
uint8_t udpDbgLevel;

/**
 * Enable or disable debug stream
 * @param *stream - output stream. Do not use network stream. NULL value disable debug stream 
 * @param level   - debug detail's level
 */
void setUdpDebug(FILE *stream, uint8_t level);
#endif



void udpInit(void);



/**
 * Send a UDP/IP packet.
 */
void udpSend(uint32_t dstIp, uint16_t dstPort, uint16_t len, uint8_t* data);


inline void netstackUDPIPProcess(udpip_hdr* packet);

inline void flushUdpQueues(void);

#endif
