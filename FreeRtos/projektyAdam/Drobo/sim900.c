#include "sim900.h"

tcpConDesc_t definedTcpConnectrions[]  __attribute__((section (".eeprom"))) =
{
    { "makgywer.vipserv.org", 2002},
    { "makgywer.vipserv.org", 2002},
    { "makgywer.vipserv.org", 2002},
    { "makgywer.vipserv.org", 2002}
};

void pwrOnHw(void)
{

}

void pwrOnSoft(void)
{

}

void pwrOffHw(void)
{

}


void pwrOffSoft(void)
{

}

void resetSim900(void)
{

}

void openTcpCon(uint8_t bufNo, uint8_t defConNo)
{
    uint16_t portNo = eeprom_read_word(&definedTcpConnectrions[defConNo].dstPortNo);
    uint8_t *eepromPtr = definedTcpConnectrions[defConNo].srcAddress;
//  eeprom_read_block()
}

void closeTcpCon(uint8_t conNo)
{

}
