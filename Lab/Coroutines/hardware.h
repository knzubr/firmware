#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include "main.h"
#include "../../freeRtos/Lib/include/protocol1.h"

#define TxStart() (PORTD |=  0x0C)
#define TxStop()  (PORTD &=  0xF3)

extern uint8_t address;
extern char bHelloResp[];

/**
 * Herdware initialization
 */
void hardwareInit(void);

void ledOn(uint8_t ledNo);
void ledOff(uint8_t ledNo);
void ledToggle(uint8_t ledNo);
char readKey(uint8_t keyNo);

/**
 * Switch on (enable) Led 1
 */
void led1on(void);
/**
 * Switch off (disable) Led 1
 */
void led1off(void);
/**
 * Toggle (change state) Led 1
 */
void led1toggle(void);
/**
 * Read key #1
 */
char readKey1(void);

/**
 * Switch on (enable) Led 1
 */
void led2on(void);
/**
 * Switch off (disable) Led 1
 */
void led2off(void);
/**
 * Toggle (change state) Led 1
 */
void led2toggle(void);
/**
 * Read key #1
 */
char readKey2(void);

/**
 * Switch on (enable) Led 1
 */
void led3on(void);
/**
 * Switch off (disable) Led 1
 */
void led3off(void);
/**
 * Toggle (change state) Led 1
 */
void led3toggle(void);
/**
 * Read key #1
 */
char readKey3(void);

/**
 * Switch on (enable) Led 1
 */
void led4on(void);
/**
 * Switch off (disable) Led 1
 */
void led4off(void);
/**
 * Toggle (change state) Led 1
 */
void led4toggle(void);
/**
 * Read key #1
 */
char readKey4(void);

#endif
