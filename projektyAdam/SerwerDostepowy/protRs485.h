#ifndef PROT485_H
#define PROT485_H

#include <avr/io.h>
#include <util/crc16.h>
#include "main.h"



void    sendPing(uint8_t addr, uint8_t pingLen, void (*sendFunc)(uint8_t));
uint8_t sprOdebrPing(uint8_t addr, uint8_t dataLen, cmdState_t *state);
uint8_t czyscBufOdb485(cmdState_t *state);

#endif