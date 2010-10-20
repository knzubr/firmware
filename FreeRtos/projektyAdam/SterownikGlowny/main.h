#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "hardware.h"
#include "hardwareConfig.h"
#include "cmdline.h"
#include "vty.h"
#include "ramdysk.h"
#include "spi.h"

#include "xitoa.h"
#include "ff.h"
#include "diskio.h"
#include "rtc.h"
#include "mpc23s17.h"
#include "mcp3008.h"
#include "ds1305.h"
#include "sensors_task.h"
#include "enc28j60.h"
#include "enc_task.h"
#include "memory_x.h"
#include "Rs485_prot.h"

#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 1
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )

#define SYSTEM_NAME "FreeRtos+"
#define S_VERSION "0.2"

///Konfiguracja Commandlina

#define STACK_SIZE_VTY          1000
#define STACK_SIZE_ENC           500
#define STACK_SIZE_SENSORS       500


#define MAKS_L_STER_ROLET       5

//#define debugRamDysk            1
//#define testZewPamiec           1



volatile timeDecoded_t czasRtc;

void initExternalMem(void) __attribute__ ((naked)) __attribute__ ((section (".init4")));


#endif
