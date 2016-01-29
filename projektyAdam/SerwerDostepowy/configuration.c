#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "nic.h"



void loadConfiguration(void)
{
  nicLoadConfig();
  ipLoadConfig();
  udpLoadConfig();
}

void saveConfiguration(void)
{
  nicSaveConfig();
  ipSaveConfig();
  udpSaveConfig();
}
