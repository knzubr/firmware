
//@{
#ifndef SPI_H
#define SPI_H

#include "main.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"


xSemaphoreHandle  xSemaphoreSpiSS;    /// Flaga blokująca jednoczesny dostęp do magistrali wielu urządzeniom
xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu

/**
 * Inicjalizacja magistrali SPI, utworzenie semaforów
 */
void    spiInit( void (*disableAllSpiDevicesFun)(void));

/**
 * Ustawia tryb pracy magistrali SPI
 * Różne urządzenia mogą pracować z różną szybkością
 * @param mode  - tryb pracy SPI
 */
void    setSpiMode(uint8_t mode);

/**
 * Ustawia szybkość magistrali SPI
 * Różne urządzenia mogą pracować z różną szybkością
 * @param speed  - tryb pracy SPI
 */
void    setSpiSpeed(uint8_t speed);

/**
 * Wyślij do magistrali SPI. Sotosowana jest blokada wirująca. 
 * Warto stosować gdy SPI działa z dużą szybkością 
 * (czas wysłania jest mniejszy od 2X czas przełączenia wątku)
 * @param uint8_t bajt do wysłania
 * @return odebrany bajt z magistrali SPI
 */
uint8_t spiSend(uint8_t data);


/**
 * Wyślij do magistrali SPI. Po rozpoczęciu transmisji przełączany jest wątek.
 * Warto stosować w przypadku wątków o niskich priorytetach lub wolnej transmisji SPI
 * @param uint8_t bajt do wysłania
 * @return odebrany bajt z magistrali SPI
 */
uint8_t spiSendSpinBlock(uint8_t data);

/**
 * Zajmuje magistralę SPI.
 * Po zajęciu magistrali należy wybrać odpowiednią linię adresową do urządzenia z którym się komunikujemy
 * W momencie, gdy magistrala jest zajęta, inne zadania nie mogą z niej korzystać.
 * Uwaga na zakleszczenia.
 */
void spiTake(void);

/**
 * Zwalnia magistralę SPI.
 * Przed zwolnieniem maigistrali należy odłączyć od niej wszystkie urządzenia
 */
void spiGive(void);

#endif
//@}