#include "hardware.h"
#include "hardwareConfig.h"
#include "spi.h"
#include <avr/io.h>

void hardwareInit(void)
{
  //DDRA = 0x00;  //External Memory
  DDRB = 0xF7;
  PORTB = 0xD1;
  /*
   0 - Sl_RST
   1 - SCK
   2 - MOSI
   3 - MISO
   4 - External SPI ASR 4
   5 - External SPI ASR 5 (DS1305)     0 - off; 1 - on 
   6 - External SPI ASR 6 (MCP3008)    0 - on;  1 - off
   7 - External SPI ASR 7 (MPC23S17)   0 - on;  1 - off
  */
  
  //DDRC = 0x00;  //External Memory

  DDRD = 0x00;
  /*
   0 - SCL
   1 - SDA
   2 - RxD USB
   3 - TxD USB
   4 - External SPI ASR 0
   5 - External SPI ASR 1
   6 - External SPI ASR 2
   7 - External SPI ASR 3
   */

  DDRE  = 0x0E;
  PORTE = 0x0C;
  /*
   0 - RxD Rs485
   1 - TxD Rs485
   2 - ENC RST
   3 - ENC CS
   4 - INT 4
   5 - INT 5
   6 - INT 6
   7 - INT Enc28j60
  */
  DDRF = 0x00;    //JTAG and A/C
  DDRG = 0x1F;
  /*
   0 - WR
   1 - RD
   2 - ALE
   3 - SD CS
   4 - RS485 TxEn
   5 - 
   6 - 
   7 - 
   */
}

void disableAllSpiDevices(void)
{ 
#if disableSpiPORTA_OR != 0
#error Port A is memory bus
  PORTA |= disableSpiPORTA_OR;
#endif
#if disableSpiPORTA_AND != 0xFF
#error Port A is memory bus
  PORTA &= disableSpiPORTA_AND;
#endif

#if disableSpiPORTB_OR != 0
  PORTB |= disableSpiPORTB_OR;
#endif
#if disableSpiPORTB_AND != 0xFF
  PORTB &= disableSpiPORTB_AND;
#endif

#if disableSpiPORTC_OR != 0
#error Port C is memory bus
  PORTC |= disableSpiPORTC_OR;
#endif
#if disableSpiPORTC_AND != 0xFF
#error Port C is memory bus
  PORTC &= disableSpiPORTC_AND;
#endif

#if disableSpiPORTD_OR != 0
  PORTD |= disableSpiPORTD_OR;
#endif
#if disableSpiPORTD_AND != 0xFF
  PORTD &= disableSpiPORTD_AND;
#endif

#if disableSpiPORTE_OR != 0
  PORTE |= disableSpiPORTE_OR;
#endif
#if disableSpiPORTE_AND != 0xFF
  PORTE &= disableSpiPORTE_AND;
#endif

#if disableSpiPORTF_OR != 0
  PORTF |= disableSpiPORTF_OR;
#endif
#if disableSpiPORTF_AND != 0xFF
  PORTF &= disableSpiPORTF_AND;
#endif

#if disableSpiPORTG_OR != 0
  PORTG |= disableSpiPORTG_OR;
#endif
#if disableSpiPORTG_AND != 0xFF
  PORTG &= disableSpiPORTG_AND;
#endif
}

void enableSpiEnc28j60(void)
{
#if ENC_SPI_CS_EN_MASK_OR != 0
  ENC_SPI_CS_PORT |= ENC_SPI_CS_EN_MASK_OR;
#endif
#if ENC_SPI_CS_EN_MASK_AND != 0xFF
  ENC_SPI_CS_PORT &= ENC_SPI_CS_EN_MASK_AND;
#endif
}

void disableSpiEnc28j60(void)
{
#if ENC_SPI_CS_EN_MASK_OR != 0
  ENC_SPI_CS_PORT &= (~ENC_SPI_CS_EN_MASK_OR);
#endif
#if ENC_SPI_CS_EN_MASK_AND != 0xFF
  ENC_SPI_CS_PORT |= (~ENC_SPI_CS_EN_MASK_AND);
#endif
}

void enableSpiSd(void)
{
#if SD_SPI_CS_EN_MASK_OR != 0
  SD_SPI_CS_PORT |= SD_SPI_CS_EN_MASK_OR;
#endif
#if SD_SPI_CS_EN_MASK_AND != 0xFF
  SD_SPI_CS_PORT &= SD_SPI_CS_EN_MASK_AND;
#endif   
}

void disableSpiSd(void)
{
#if SD_SPI_CS_EN_MASK_OR != 0
  SD_SPI_CS_PORT &= (~SD_SPI_CS_EN_MASK_OR);
#endif
#if SD_SPI_CS_EN_MASK_AND != 0xFF
  SD_SPI_CS_PORT |= (~SD_SPI_CS_EN_MASK_AND);
#endif  
}

void enableSpiMPC23S17(void)
{
#if MCP23S17_SPI_CS_EN_MASK_OR != 0
  MCP23S17_SPI_CS_PORT |= MCP23S17_SPI_CS_EN_MASK_OR;
#endif
#if MCP23S17_SPI_CS_EN_MASK_AND != 0xFF
  MPC23S17_SPI_CS_PORT &= MPC23S17_SPI_CS_EN_MASK_AND;
#endif
}

void disableSpiMPC23S17(void)
{
#if MCP23S17_SPI_CS_EN_MASK_OR != 0
  MCP23S17_SPI_CS_PORT &= (~MCP23S17_SPI_CS_EN_MASK_OR);
#endif
#if MCP23S17_SPI_CS_EN_MASK_AND != 0xFF
  MPC23S17_SPI_CS_PORT |= (~MPC23S17_SPI_CS_EN_MASK_AND);
#endif
}

#define MCP3008_SPCR_OR_MASK ((1<<SPR1)|(1<<SPR0))
void enableSpiMCP3008(void)
{
  SPCR |= MCP3008_SPCR_OR_MASK;
#if MCP3008_SPI_CS_EN_MASK_OR != 0
  MCP3008_SPI_CS_PORT |= MCP3008_SPI_CS_EN_MASK_OR;
#endif
#if MCP3008_SPI_CS_EN_MASK_AND != 0xFF
  MCP3008_SPI_CS_PORT &= MCP3008_SPI_CS_EN_MASK_AND;
#endif  
}

void disableSpiMCP3008(void)
{
  SPCR &= ~MCP3008_SPCR_OR_MASK;
  #if MCP3008_SPI_CS_EN_MASK_OR != 0
  MCP3008_SPI_CS_PORT &= (~MCP3008_SPI_CS_EN_MASK_OR);
#endif
#if MCP3008_SPI_CS_EN_MASK_AND != 0xFF
  MCP3008_SPI_CS_PORT |= (~MCP3008_SPI_CS_EN_MASK_AND);
#endif
}

//#define DS_SPCR_OR_MASK            (1<<SPR1)
#define DS_SPCR_OR_MASK ((1<<CPHA)|(1<<SPR0))
//#define DS_SPCR_OR_MASK (1<<CPHA)
void enableSpiDs1305(void)
{
  SPCR |= DS_SPCR_OR_MASK;
//spiSetCPHA();
#if DS1305_SPI_CS_EN_MASK_OR != 0
  DS1305_SPI_CS_PORT |= DS1305_SPI_CS_EN_MASK_OR;
#endif
#if DS1305_SPI_CS_EN_MASK_AND != 0xFF
  DS1305_SPI_CS_PORT &= DS1305_SPI_CS_EN_MASK_AND;
#endif
}

void disableSpiDs1305(void)
{
  SPCR &= ~DS_SPCR_OR_MASK;
//  spiClearCPHA();
#if DS1305_SPI_CS_EN_MASK_OR != 0
  DS1305_SPI_CS_PORT &= (~DS1305_SPI_CS_EN_MASK_OR);
#endif
#if DS1305_SPI_CS_EN_MASK_AND != 0xFF
  DS1305_SPI_CS_PORT |= (~DS1305_SPI_CS_EN_MASK_AND);
#endif  
}
