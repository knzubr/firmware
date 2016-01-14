#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define Rs485TxStart() (PORTG |=  0x10)
#define Rs485TxStop()  (PORTG &=  0xEF)


/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);

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

#endif

