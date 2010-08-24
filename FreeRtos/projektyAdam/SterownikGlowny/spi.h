#ifndef SPI_H
#define SPI_H

#include "main.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

#define SS_ON_PORTD_MASK    0xFF
#define DISABLE_SS_ON_PORTD 0xFF

//Enc28j60        PE3 - SS
#define SS_ON_PORTE_MASK    0x08
#define DISABLE_SS_ON_PORTE 0x08

//SD              PG3 - CS
#define SS_ON_PORTG_MASK    0x08
#define DISABLE_SS_ON_PORTG 0x08


xSemaphoreHandle  xSemaphoreSpiSS;
xQueueHandle      xSpiRx;

/**
 * Inicjalizacja magistrali SPI, utworzenie semaforów
 */
void    spiInit                      (void);

/**
 * Wyślij do magistrali SPI. Sotosowana jest blokada wirująca. 
 * Warto stosować gdy SPI działa z dużą szybkością 
 * (czas wysłania jest mniejszy od 2X czas przełączenia wątku)
 * @param uint8_t bajt do wysłania
 * @return odebrany bajt z magistrali SPI
 */
uint8_t spiSend                      (uint8_t data);


/**
 * Wyślij do magistrali SPI. Po rozpoczęciu transmisji przełączany jest wątek.
 * Warto stosować w przypadku wątków o niskich priorytetach lub wolnej transmisji SPI
 * @param uint8_t bajt do wysłania
 * @return odebrany bajt z magistrali SPI
 */
uint8_t spiSendSpinBlock             (uint8_t data);

void    disableAllDevices            (void);
void    enableExternalSpiDevice      (uint8_t devices);
void    enableExternalSpiDevicePortA (uint8_t mask);
void    enableExternalSpiDevicePortB (uint8_t mask);
void    enableEnc                    (void);
void    enableSD                     (void);



#endif