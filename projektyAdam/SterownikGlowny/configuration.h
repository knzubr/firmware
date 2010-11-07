#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "enc28j60.h"
#include "hardware.h"

extern uint8_t mymac[6];
extern uint8_t myip[4];
extern uint8_t mask;

extern struct lockerSensor *lockSensors;


void loadConfiguration(void);
void saveConfiguration(void);


#endif