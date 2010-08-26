/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : MCP3008 / MCP3004 A/C converter driver
* Author     : Adam Kaliszan adam.kaliszan@gmail.com
* Copyright  : GPL V2
*
*This driver provides:
* - read/set operation
*Driver uses mutexes and is condition race free. Function can be invoken by any thread.
*****************************************************************************/
//@{

#ifndef MCP3008_H
#define MCP3008_H

#include <inttypes.h>
#include "main.h"
#include "spi.h"


struct MCP3008_config
{
  uint8_t (*spiSend)(uint8_t data); /// wskaźnik do funkcji, która wysyła i odbiera dane przez SPI
  void (*spiEnableMcp)(void);
  void (*spiDisablMcp)(void);  
};

struct MCP3008_config MCP3008_global;

// functions
/**
 * Przygotowanie globalnej struktury, w której poustawiane wskaźniki na odpowiednie funkcje
 * @param *spiSendFunc       - wskaźnik do funkcji, która wysyła przez SPI (uint8_t spiSend(uint8_t data))
 * @param *spiEnableMcpFunc  - wskaźnik do funkcji, która podłącza przetwornik A/C do magistrali SPI (void enableSpi(void))
 * @param *spiDisableMcpFunc - wskaźnik do funkcji, która odłącza przetwornik A/C od magistrali SPI (void disableSpi(void)) 
 */
void MCP3008_init(uint8_t (*spiSendFunc)(uint8_t data), void (*spiEnableMcpFunc)(void), void (*spiDisableMcpFunc)(void));

/**
 * Zwraca watość próbki dla pojedynczego wejścia
 * @param inputNo - numer wejścia (0-7).
 */
uint16_t MCP3008_getSampleSingle(uint8_t inputNo);

/**
 * Zwraca watość próbki dla różnicowego wejścia
 * @param inputNo - numer wejścia (0-7).
 */
uint16_t MCP3008_getSampleDiff(uint8_t inputNo);

#endif
//@}
