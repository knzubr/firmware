#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>

#define Rs485TxStart() (PORTG |=  0x10)
#define Rs485TxStop()  (PORTG &=  0xEF)


/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);


#endif
