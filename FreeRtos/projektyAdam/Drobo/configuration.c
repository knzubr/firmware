#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"


//static uint8_t trAddressEeprom[256] __attribute__((section (".eeprom"))) = {0x00, 0x01};

void loadConfiguration(void)
{
  //eeprom_read_block(translateTable, trAddressEeprom, 256);
}

void saveConfiguration(void)
{
  //eeprom_write_block(translateTable, trAddressEeprom, 256);
}
