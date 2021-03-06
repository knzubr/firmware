#include "configuration.h"
#include <avr/eeprom.h>

static uint8_t myip_eep[4]  __attribute__((section (".eeprom"))) = {10,0,0,24};
static uint8_t mask_eep     __attribute__((section (".eeprom"))) = 8;
static uint8_t mymac_eep[6] __attribute__((section (".eeprom"))) = {0x54,0x55,0x58,0x10,0x00,0x24};

void loadConfiguration(void)
{
  eeprom_read_block(myip,  myip_eep,  4);
  eeprom_read_block(mymac, mymac_eep, 6);
  mask = eeprom_read_byte(&mask_eep);
}

void saveConfiguration(void)
{
  eeprom_update_block(myip,  myip_eep,  4);
  eeprom_update_block(mymac, mymac_eep, 6);
  eeprom_update_byte(&mask_eep, mask);
}
