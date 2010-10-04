#ifndef RS485_PROT_H
#define RS485_PROT_H

#include <util/crc16.h>
#include "main.h"
#include "protocol1.h"


void takeRs485(void)                               __attribute__ ((weak));
void releaseRs485(void)                            __attribute__ ((weak));
void uartRs485SendByte(uint8_t c)                  __attribute__ ((weak));
uint8_t rs485Receive(uint8_t *c, uint8_t timeout)  __attribute__ ((weak));
uint8_t flushRs485RecBuffer(void)                  __attribute__ ((weak));

// ********************* Hiden Functions ***************************************************

uint8_t rs485ping(uint8_t devAddr);

uint8_t rs485xModemFlash(struct ramPlikFd *file, uint8_t devAddr);
uint8_t rs485curtainUp(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos);
uint8_t rs485curtainDown(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos);


#endif