#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "memory_x.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"

#include "mpc23s17.h"
#include "mcp3008.h"
#include "spiXmega.h"

#define Rs485TxStart() (PORTF.OUT |=  0x20)
#define Rs485TxStop()  (PORTF.OUT &=  0xDF)

struct lockerSensor
{
  uint8_t  enabled;
  uint16_t threshold;
  uint16_t acVal;
  uint8_t  locked;
};


struct lockerSensor *lockSensors;


/**
 * Hardware initialize
 */
void hardwareInit(void);

/**
 * Initialization of memory for lockers state structs
 */
void LockersMemInit(void);


// ************************ I/O module *********************************


// ************************ Obsługa Rs485 ******************************
void takeRs485(void);
void releaseRs485(void);
void rs485Send(uint8_t c);
uint8_t rs485Receive(uint8_t *c, uint8_t timeout);

// ************************ Obsługa SPI ********************************
uint8_t spiSend(uint8_t data);
uint8_t spiSendSpinBlock(uint8_t data);


void disableAllSpiDevices(void);

void spiEnableEnc28j60(void);
void spiDisableEnc28j60(void);

void enableSpiSd(void);
void disableSpiSd(void);

void enableSpiMPC23S17(void);
void disableSpiMPC23S17(void);

void enableSpiMCP3008(void);
void disableSpiMCP3008(void);

void spiEnableDS1305(void);
void spiDisableDS1305(void);

void enableSpiMCP4150(void);
void disableSpiMCP4150(void);

uint8_t spiSendENC(uint8_t data);
uint8_t spiSendSpinBlockENC(uint8_t data);

#endif

