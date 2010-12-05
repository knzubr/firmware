#ifndef UDP_H
#define UDP_H

#include <avr/pgmspace.h>
#include "softwareConfig.h"
#include "ip.h"


#if UDP_DEBUG
#include <stdio.h>
#include <avr/pgmspace.h>
#endif

extern struct ipConfig IpMyConfig;

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


void netstackUDPIPProcess(uint16_t len, udpip_hdr* packet);

#endif
