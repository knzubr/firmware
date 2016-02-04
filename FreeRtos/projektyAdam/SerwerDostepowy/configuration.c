#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "nic.h"

uint32_t udpIpDst_eep   __attribute__((section (".eeprom"))) = ((uint32_t)UDP_DST_IP4   << 24) + ((uint32_t)UDP_DST_IP3   <<16) + ((uint32_t)UDP_DST_IP2   <<8) + UDP_DST_IP1;
uint16_t udpPortDstEep  __attribute__((section (".eeprom"))) = HTONS(UDP_DST_PORT);
uint16_t udpPortSrcEep  __attribute__((section (".eeprom"))) = HTONS(UDP_SRC_PORT);

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
