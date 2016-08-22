#ifndef VTY_H
#define VTY_H

#include "main.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/crc16.h>

#include "memory_x.h"
#include "configuration.h"
#include "cmdline.h"
#include "FreeRTOS.h"
#include "hc12.h"

#include "../../freeRtos/Source/include/FreeRTOS.h"
#include "../../freeRtos/Source/include/croutine.h"
#include "../../freeRtos/Source/include/queue.h"
#include "../../freeRtos/Source/include/task.h"


extern xSemaphoreHandle Hc12semaphore;

void VtyInit(cmdState_t *state, FILE *stream);
void printErrorInfo(cmdState_t *state);
void printStatus(FILE *stream);

extern FILE hc12Stream;
extern FILE hc12FakeStream;

enum errorType
{
  AllOK  = 0,
  noFile = 1,
  xModemFrameStartTimeout = 2,
  xModemByteSendTimeout = 3,
  xModemWrongFrameNo = 4,
  xModemFrameFrameNoCorrectionNotMatch = 5,
  xModemFrameCrc = 6,
  xModemRemoteSideCan = 7,
  xModemUnknownResponse = 8,
  noRemoteDevice = 9,
  bootloaderNotResponding = 10,
  cantOpenFile = 11
};

typedef enum errorType errorType_t;

#endif