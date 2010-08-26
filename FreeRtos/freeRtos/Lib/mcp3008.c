/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : MPC23s17 parrarel I/O 16 bit port driver
* Author     : Adam Kaliszan adam.kaliszan@gmail.com
* Copyright  : GPL V2
*
*This driver provides:
* - read/set operation
*Driver uses mutexes and is condition race free. Function can be invoken by any thread.
*****************************************************************************/
//@{
#include <avr/io.h>
#include <util/delay.h>
#include "mcp3008.h"
#include "spi.h"


void MCP3008_init(uint8_t (*spiSendFunc)(uint8_t data), void (*spiEnableMcpFunc)(void), void (*spiDisableMcpFunc)(void))
{
  MCP3008_global.spiSend       = spiSendFunc;
  MCP3008_global.spiEnableMcp  = spiEnableMcpFunc;
  MCP3008_global.spiDisablMcp  = spiDisableMcpFunc;
}

uint16_t MCP3008_getSampleDiff(uint8_t inputNo)
{
  uint8_t resultLo;
  uint8_t resultHi;
  inputNo = inputNo << 4;
  inputNo &= 0x70;
  
  spiTake();
  MCP3008_global.spiEnableMcp();
  
  MCP3008_global.spiSend(0x01);                //Start
  resultHi = MCP3008_global.spiSend(inputNo);  //DIFF/!SGL A2 A1 A0 X X X X
  resultLo = MCP3008_global.spiSend(0);        //X X X X X X X X
  
  MCP3008_global.spiDisablMcp(); 
  spiGive();
  
  resultHi &= 0x03;
  return resultHi << 8 | resultLo;
}

uint16_t MCP3008_getSampleSingle(uint8_t inputNo)
{
  uint8_t resultLo;
  uint8_t resultHi;
  inputNo = inputNo << 4;
  inputNo &= 0x70;
  inputNo |= 0x80;
  
  spiTake();
  MCP3008_global.spiEnableMcp();
  
  MCP3008_global.spiSend(0x01);                //Start
  resultHi = MCP3008_global.spiSend(inputNo);  //DIFF/!SGL A2 A1 A0 X X X X
  resultLo = MCP3008_global.spiSend(0);        //X X X X X X X X
  
  MCP3008_global.spiDisablMcp(); 
  spiGive();
  
  resultHi &= 0x03;
  return resultHi << 8 | resultLo;
}
//@}