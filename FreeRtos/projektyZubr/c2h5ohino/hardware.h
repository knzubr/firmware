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
#include "spi.h"


/**
 * Hardware initialize
 */
void hardwareInit(void);

#endif


