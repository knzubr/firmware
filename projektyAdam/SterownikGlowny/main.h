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
#include "cmdline.h"
#include "vty.h"
#include "protRs485.h"
#include "ramdysk.h"
//#include "enc_task.h"
//#include "spiStd.h"

#include "xitoa.h"
#include "ff.h"
#include "diskio.h"
#include "rtc.h"
//#include "mpc23s17.h"

#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 1
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )


///Konfiguracja Commandlina
#define STACK_SIZE_VTY          1000

#define STACK_SIZE_ENC           500


#define MAKS_L_STER_ROLET       5

//#define debugRamDysk            1
//#define testZewPamiec           1

struct sterRolet
{
  uint8_t adres;
  uint8_t stan;
  uint8_t roleta[2];
};

#endif
