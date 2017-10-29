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

#define READ0  PIND & 0x01
#define READ1  PIND & 0x02
#define READ2  PIND & 0x04
#define READ3  PIND & 0x08
#define READ4  PIND & 0x10
#define READ5  PIND & 0x20
#define READ6  PIND & 0x40
#define READ7  PIND & 0x80
#define READ8  PINB & 0x01
#define READ9  PIND & 0x02
#define READ10 PIND & 0x04
#define READ11 PIND & 0x08
#define READ12 PIND & 0x10
#define READ13 PIND & 0x20


/**
 * Hardware initialize
 */
void hardwareInit(void);

void adcSignle(uint8_t chanNo);
uint16_t adcRead(void);

#endif


