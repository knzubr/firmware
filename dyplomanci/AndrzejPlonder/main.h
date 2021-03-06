#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/crc16.h>

#include "memory_x.h"
//#include "Rs485_prot.h"

#include "../../freeRtos/Source/include/FreeRTOS.h"
#include "../../freeRtos/Source/include/croutine.h"
#include "../../freeRtos/Source/include/queue.h"
#include "../../freeRtos/Source/include/task.h"
#include "queueStream.h"
#include "cli_task.h"
#include "serial.h"
#include "pelco_d.h"

#include "hardwareConfig.h"
#include "softwareConfig.h"

#include "hardware.h"
#include "spiXmega.h"

#include "lcd.h"

#include "cmdline.h"
#include "vty.h"




#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 1

#define SYSTEM_NAME "FreeRtos+"
#define S_VERSION "0.31"


void my_init_clock (void) __attribute__ ((naked))  __attribute__ ((section (".init0")));
void initExternalMem(void) __attribute__ ((naked)) __attribute__ ((section (".init4")));

#endif
