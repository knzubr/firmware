#ifndef MAIN_H
#define MAIN_H


#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <string.h>
#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 100

#endif
