/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : MPC23s17 parrarel I/O 16 bit port driver
* Author     : Adam Kaliszan
* Copyright: GPL V2
*
*This driver provides:
* - read/set operation
*Driver uses mutexex and is condition race free. Function can be invoken by any thread.
*****************************************************************************/
//@{
#include <avr/io.h>
#include <util/delay.h>
#include "mpc23s17.h"
#include "spi.h"


void MPC23S17_init(uint8_t (*spiSendFunc)(uint8_t data), void (*spiEnableMpcFunc)(void), void (*spiDisableMpcFunc)(void))
{
  MPC_global.spiSend       = spiSendFunc;
  MPC_global.spiEnableMpc  = spiEnableMpcFunc;
  MPC_global.spiDisableAll = spiDisableMpcFunc;
}

void MPC23s17SetDirA(uint8_t portAdir, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  MPC_global.spiEnableMpc();
  MPC_global.spiSend(addr);
  MPC_global.spiSend(B0_IODIRA);
  MPC_global.spiSend(portAdir);  
  MPC_global.spiDisableAll(); 
  spiGive();
}

void MPC23s17SetDirB(uint8_t portBdir, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  MPC_global.spiEnableMpc();
  MPC_global.spiSend(addr);
  MPC_global.spiSend(B0_IODIRB);
  MPC_global.spiSend(portBdir);  
  MPC_global.spiDisableAll(); 
  spiGive();
}

void MPC23s17SetPortA(uint8_t portAout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  MPC_global.spiEnableMpc();
  MPC_global.spiSend(addr);
  MPC_global.spiSend(B0_OLATA);
  MPC_global.spiSend(portAout);  
  MPC_global.spiDisableAll(); 
  spiGive();
}

void MPC23s17SetPortB(uint8_t portBout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  MPC_global.spiEnableMpc();
  MPC_global.spiSend(addr);
  MPC_global.spiSend(B0_OLATB);
  MPC_global.spiSend(portBout);  
  MPC_global.spiDisableAll(); 
  spiGive();
}

uint8_t MPC23s17ReadPortA(uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x41;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  MPC_global.spiEnableMpc();
  MPC_global.spiSend(addr);
  MPC_global.spiSend(B0_GPIOA);
  uint8_t result = MPC_global.spiSend(addr);  
  MPC_global.spiDisableAll(); 
  spiGive();

  return result; 
}

uint8_t MPC23s17ReadPortB(uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x41;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  MPC_global.spiEnableMpc();
  MPC_global.spiSend(addr);
  MPC_global.spiSend(B0_GPIOB);
  uint8_t result = MPC_global.spiSend(addr);  
  MPC_global.spiDisableAll(); 
  spiGive();

  return result; 
}
