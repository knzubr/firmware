#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "memory_x.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"

#include "mpc23s17.h"
#include "mcp3008.h"
#include "spiXmega.h"

#define Rs485TxStart() (PORTF.OUT |=  0x20)
#define Rs485TxStop()  (PORTF.OUT &=  0xDF)

/**
 * Hardware initialize
 */
void hardwareInit(void);

void Rs485TxStop_2(void);
void Rs485TxStart_2(void);

#endif

