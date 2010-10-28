#ifndef RS485_PROT_H
#define RS485_PROT_H

#include "main.h"
#include <stdio.h>
#include <util/crc16.h>
#include "ramdysk.h"
#include "protocol1.h"

//@{
struct rs485device
{
  uint8_t adres;         /// Device Address on Rs485 bus
  uint8_t stan;          /// Device state
  uint8_t version[5];    /// Firmware Version  
};

struct sterRolet 
{
  struct rs485device rsDeviceState __attribute__ ((packed));
  uint8_t roleta[2];     /// Rollers states
};

// ******************************** Hardware specyfic function ***********************
void    takeRs485(void)                           __attribute__ ((weak));
void    releaseRs485(void)                        __attribute__ ((weak));
void    uartRs485SendByte(uint8_t c)              __attribute__ ((weak));
uint8_t rs485Receive(uint8_t *c, uint8_t timeout) __attribute__ ((weak));
uint8_t flushRs485RecBuffer(void)                 __attribute__ ((weak));

/**
 * Sends Ping and waits for response
 * @param devAddr - device address
 * @return 0 - All OK
 */
uint8_t rs485ping(uint8_t devAddr);

/**
 * Sends Ping and waits for response
 * @param devAddr - device address
 * @param *data   - data pointer
 * @param maxSize - maximum size of data that can be written
 * @return 0 - All OK, 1 - wrong size, 2 - no response
 */
uint8_t rs485hello(uint8_t devAddr, void *data, uint8_t maxSize);



/**
 * Flash remote device connected to Rs485 bus
 * @param file    - file descriptor with firmware
 * @param devAddr - device address
 * @return 0 - All OK
 */
uint8_t rs485xModemFlash(struct ramPlikFd *file, uint8_t devAddr);

/**
 * Move curtain up
 * @param deviceAddr - remote module address
 * @param curtainNo  - number of curtain (0 or 1)
 * @param pos        - position of curtain
 * @return 0 - All OK
 */
uint8_t rs485curtainUp(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos);

/**
 * Move curtain down
 * @param deviceAddr - remote module address
 * @param curtainNo  - number of curtain (0 or 1)
 * @param pos        - position of curtain
 * @return 0 - All OK
 */
uint8_t rs485curtainDown(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos);

//@}
#endif
