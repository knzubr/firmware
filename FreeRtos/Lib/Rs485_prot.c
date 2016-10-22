#include "Rs485_prot.h"
#include <util/crc16.h>
#include <stdio.h>

#if LANG_EN
#include "Rs485_prot_en.h"
#define PRINT_RS485_DEVICE 1
#endif

#if LANG_PL
#include "Rs485_prot_pl.h"
#define PRINT_RS485_DEVICE 1
#endif

void rollersMemInit(void)
{

}

#ifdef PRINT_RS485_DEVICE
/**
 * Prints list of Rs485 devices
 * @param stream - outpuf stream
 * @return number of printed devices
 */
uint8_t printRs485devices(FILE *stream);
#endif /*PRINT_RS485_DEVICE*/


/**
 * Sends Ping and waits for response
 * @param devAddr - device address
 * @return 0 - All OK
 */
uint8_t rs485ping(uint8_t devAddr)
{

}


void sendSettings(uint8_t addr, uint8_t value)
{

}

void saveSettings(uint8_t addr)
{

}

/**
 * Sends Hello message to the roller driver and reads its response
 * @param devAddr - device address
 * @return 0 - All OK, 1 - no space in memory to write info, 2 - no response, 3 - wrong address
 */
uint8_t rs485rollerHello(uint8_t devAddr)
{

}

/**
 * Flash remote device connected to Rs485 bus
 * @param file    - file descriptor with firmware
 * @param devAddr - device address
 * @return 0 - All OK
 */
uint8_t rs485xModemFlash(struct ramPlikFd *file, uint8_t devAddr, FILE *debStr)
{

}

/**
 * Move curtain up
 * @param deviceAddr - remote module address
 * @param curtainNo  - number of curtain (0 or 1)
 * @param pos        - position of curtain
 * @return 0 - All OK
 */
uint8_t rs485curtainUp(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos)
{

}

uint8_t rs485Led(uint8_t deviceAddr, uint8_t ledNo, uint8_t time)
{

}


/**
 * Move curtain down
 * @param deviceAddr - remote module address
 * @param curtainNo  - number of curtain (0 or 1)
 * @param pos        - position of curtain
 * @return 0 - All OK
 */
uint8_t rs485curtainDown(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos)
{


}
