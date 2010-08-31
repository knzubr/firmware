#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>

#define Rs485TxStart() (PORTG |=  0x10)
#define Rs485TxStop()  (PORTG &=  0xEF)


/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);

void disableAllSpiDevices(void);

void enableSpiEnc28j60(void);
void disableSpiEnc28j60(void);

void enableSpiSd(void);
void disableSpiSd(void);

void enableSpiMPC23S17(void);
void disableSpiMPC23S17(void);

void enableSpiMCP3008(void);
void disableSpiMCP3008(void);

void enableSpiDs1305(void);
void disableSpiDs1305(void);

#endif
