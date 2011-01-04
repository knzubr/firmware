#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include "main.h"
#include "../../freeRtos/Lib/include/protocol1.h"

#define TxStart() (PORTD |=  0x0C)
#define TxStop()  (PORTD &=  0xF3)

extern uint8_t adres;
extern char bHelloResp[];

/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);


/**
 * Włącznenie przekaźnika - rozpoczęcie podnoszenia rolety 1
 */
void roletawGore(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie opuszczania rolety 1
 */
void roletawDol(void);

/**
 * Wyłącznenie przekaźników - zatrzymanie rolety 1
 */
void roletaStop(void);

/**
 * Odczyt stanu klawisza do podnoszenie rolety 1
 */
char czytKlawiszRolwGore(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 1
 */
char czytKlawiszRolwDol(void);

#endif
