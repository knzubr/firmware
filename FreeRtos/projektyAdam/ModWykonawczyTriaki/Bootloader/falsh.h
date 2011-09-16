#ifndef FLASSH_H
#define FLASSH_H

#include <avr/boot.h>
#include <avr/pgmspace.h>

extern uint16_t        FlashAddr;


void write_one_page(unsigned char *buf);




#endif