#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
//#include <avr/interrupt.h>

//#include "hardwareConfig.h"
//#include "softwareConfig.h"




/**
 * Hardware initialize
 */
void hardwareInit(void);

void offHbridge(void);

void forwardA(uint8_t left, uint8_t right);
void backwordA(uint8_t left, uint8_t right);
void rotateLeftA(uint8_t left, uint8_t right);
void rotateRightA(uint8_t left, uint8_t right);

void forwardB(uint8_t left, uint8_t right);
void backwordB(uint8_t left, uint8_t right);
void rotateLeftB(uint8_t left, uint8_t right);
void rotateRightB(uint8_t left, uint8_t right);



#endif

