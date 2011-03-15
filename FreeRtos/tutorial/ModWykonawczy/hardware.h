#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include "main.h"

#define TxStart() (PORTD |=  0x0C)
#define TxStop()  (PORTD &=  0xF3)


/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);

/**
 * Zapalenie diody 1
 */
void zapal1(void);

/**
 * Zgaszenie diody 1
 */
void zgas1(void);

/**
 * Zapalenie diody 2
 */
void zapal2(void);

/**
 * Zgaszenie diody 2
 */
void zgas2(void);

/**
 * Zapalenie diody 3
 */
void zapal3(void);

/**
 * Zgaszenie diody 3
 */
void zgas3(void);

/**
 * Zapalenie diody 4
 */
void zapal4(void);

/**
 * Zgaszenie diody 4
 */
void zgas4(void);

/**
 * Zapalenie diody kontrolej 1
 */
void Led1On(void);

/**
 * Zmiana stanu diody kontrolej 1
 */
void Led1Toggle(void);

/**
 * Gaszenie diody kontrolej 1
 */
void Led1Off(void);

/**
 * Zapalenie diody kontrolej 2
 */
void Led2On(void);

/**
 * Zmiana stanu diody kontrolej 2
 */
void Led2Toggle(void);

/**
 * Gaszenie diody kontrolej 2
 */
void Led2Off(void);

/**
 * Zapalanie diody
 * @param numer diody (zaczyna się od 0)
 */
void zapal(int nr);

/**
 * Wyłączanie diody
 * @param numer diody (zaczyna się od 0)
 */
void zgas(int nr);

/**
 * Czytaj stan klawisza
 * @param numer klawisza (zaczyna się od 0)
 */
char czytajKlawisz(int nr);
#endif
