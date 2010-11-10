#ifndef ENC_TASK_H
#define ENC_TASK_H 1

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "avr_compat.h"
//#include "net.h"
#include "hardwareConfig.h"

#define MYWWWPORT    80
#define MYTELNETPORT 23
#define MYUDPPORT    1200

uint8_t mymac[6];
uint8_t myip[4];
uint8_t mask;

uint8_t verify_password(char *str);

/**
 * takes a string of url address and process it
 * @param str - part of URL string
 * @param [out] - filename or command
 * @return 1 - read from ram dysk, 2 - read from SD card, 3 - execute CLI command
 */
uint8_t analyse_get_url (const char *str, char *fname);

/**
 * Enc28j60 task
 */
void encTask(void *pvParameters);

/**
 * initialize enc28j60
 */
void enc28j60chipInit(void);

extern struct Enc28j60_config Enc28j60_global;
#endif
