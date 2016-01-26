#ifndef SPI_TASK_H
#define SPI_TASK_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "enc28j60.h"
#include "net.h"
#include "nic.h"
#include "ip.h"
#include "arp.h"
#include "udp.h"
#include "tcp.h"
#include "icmp.h"
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"

#include <util/crc16.h>

extern xQueueHandle xSpi2SerialTx[];
extern xQueueHandle xSpi2SerialRx[];


enum REC_STATE
{
    ST_SYNC,
    ST_CMD,
    ST_LEN,
    ST_DATA,
    ST_CRC_LO,
    ST_CRC_HI
} ;

void spiTask ( void *pvParameters );
void XpNetReceiverTask ( void *pvParameters );

void handleSpiDev(uint8_t spiDevNo);
void initQeuesSpi2Serial(void);

void flushSpi2SerialRxBuffers(void);


/** Format wiadomosci spi
Bajt danych odebrany przez porty szeregowych wsyłany jest jako 2 bajty przez SPI. Pierwszy bajt zawiera bity tanych 3-0, natomiast drugi bajt bity danych 7-4. Istotna jest kolejność

 * bit  | znaczenie
 * 7, 6 | 0x dane, 10 pusta ramka 11 rozkaz,

Format danych
 * 6    |
 * 5    | portNo
 * 4    | 1 in data are stored bits 7-4, 0 in data are stored bits 3-0
 * 3-0  | data bits


Format rozkazu
 * 5    | 1 power On, 0 power Off
 * 4    | portNo
 * 3-0  | data bits

Pusta ramka
 0xCF

 */


#endif // SPI_TASK_H
