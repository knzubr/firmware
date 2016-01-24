#ifndef PELCO_D_H
#define PELCO_D_H

#include "main.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

struct pelcoMsg
{
    uint8_t sync;    //0xFF
    uint8_t addr;
    uint8_t cmd[2];
    uint8_t opt[2];
    uint8_t crc;
};

extern xQueueHandle         pelcoMessages;
extern xQueueHandle         rs485_2tx;

void pelcoInit()

#endif // PELCO_D_H
