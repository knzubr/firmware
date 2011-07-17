#ifndef ENC_TASK_H
#define ENC_TASK_H 1

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "enc28j60.h"
#include "net.h"
#include "nic.h"
#include "ip.h"
#include "arp.h"
#include "udp.h"
#include "tcp.h"
#include "icmp.h"
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"


/**
 * Enc28j60 task
 */
void encTask(void *pvParameters);

extern nicState_t nicState;
#endif
