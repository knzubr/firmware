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
  spiTake();
  MPC_global.spiEnableMpc();
  
  MPC_global.spiDisableAll();  
  spiGive();
}

void MPC23s17SetDirB(uint8_t portBdir, uint8_t addr)
{
  spiTake();
  MPC_global.spiEnableMpc();
  
  MPC_global.spiDisableAll();
  spiGive();
}

void MPC23s17SetPortA(uint8_t portAout, uint8_t addr)
{
  spiTake();
  MPC_global.spiEnableMpc();
  
  MPC_global.spiDisableAll(); 
  spiGive();
}

void MPC23s17SetPortB(uint8_t portBout, uint8_t addr)
{
  spiTake();
  MPC_global.spiEnableMpc();
  
  MPC_global.spiDisableAll();
  spiGive();
}

uint8_t MPC23s17ReadPortA(uint8_t addr)
{
  spiTake();
  MPC_global.spiEnableMpc();
  
  MPC_global.spiDisableAll();
  spiGive();
  return 0; 
}

uint8_t MPC23s17ReadPortB(uint8_t addr)
{
  spiTake();
  MPC_global.spiEnableMpc();
  
  MPC_global.spiDisableAll();
  spiGive();
  return 0;
}
