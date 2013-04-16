#include "hardware.h"

#if LANG_EN
#include "hardware_en.h"
#endif

#if LANG_PL
#include "hardware_pl.h"
#endif

xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
xQueueHandle      xSpiRxEnc;
// napisać funkcje do konfiguracji zew pam s-ram
void hardwareInit(void)
{
  //DDRA = 0x00;  //External Memory
  portENTER_CRITICAL();
  xSpiRx          = xQueueCreate(1, 1);
  portEXIT_CRITICAL();

 //DDRA = 0x00;  //External Memory
  portENTER_CRITICAL();
  xSpiRx          = xQueueCreate(1, 1);
  portEXIT_CRITICAL();
  /* Port B
7- JTAG TD0
6- JTAG TCK
5- JTAG TDI
4- JTAG TMS
5:2 - LCD D4:D7
0 - ENC28J60 CS
*/
PORTB.DIR=0x01;
/* Port C
7- ENC28J60 SCK
6- ENC28J60 SO
5- ENC28J60 SI
4- SD_card CS
3- ST2378 OE
1- I2C SCL
0- I2C SDA
*/
PORTC.DIR=0xA0;
/* PORTD
7- SD SCK
6- SD S0
5- SD SI
4- ZL SPI SLRST
3- FT232 TXT-WY
2- FT232 RXT-WE
1- ENC28J60 RST
0- ENC28J60 INT
*/
PORTD.DIR=0xBA;
/* PORTE
7:0- ZL SPI A7:A0
*/
PORTE.DIR=0xFF;
/* PORTF
7- INT
6- INT
5- RS485 DE/RE
4- INT
3- RS485 TXD
2- RS485 RXD
*/
PORTF.DIR=0x28;
/* PORTH
6:4- EBI A18:A16
2-   EBI ALE
1-   EBI RE
0-   EBI WE
*/
PORTH.DIR=0xFF;
/* PORTJ
7:0- EBI D7:D0/A0:A7
*/
PORTJ.DIR=0xFF;
/* PORTK
7:0- EBI A8:A15
*/
PORTK.DIR=0xFF;
}

void LockersMemInit(void)
{
  lockSensors = xmalloc(4 * sizeof(struct lockerSensor));
}

uint8_t printLockers(FILE *stream)
{
  uint8_t i;
  uint8_t result = 0;
  struct lockerSensor *tmpLock = lockSensors;
  for (i=1; i<=4; i++)
  {
    if (tmpLock->enabled)
    {
      fprintf_P(stream, statusLockerSensDescStr, i);
      if (tmpLock->threshold > tmpLock->acVal)
        fprintf_P(stream, statusLockerOpenStr);
      else
        fprintf_P(stream, statusLockerCloseStr);
      fprintf_P(stream, statusLockerSensAdditionalDescStr, tmpLock->threshold, tmpLock->acVal);
      result++;
    }
    tmpLock++;
  }
  return result;
}

void checkLockerSensors(void)
{
  if (lockSensors[0].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_1_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[0].acVal = MCP3008_getSampleSingle(LOCK_SENS_1_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_1_LIGHT, 0);
    lockSensors[0].locked = (lockSensors[0].acVal > lockSensors[0].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
  
  if (lockSensors[1].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_2_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[1].acVal = MCP3008_getSampleSingle(LOCK_SENS_2_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_2_LIGHT, 0);
    lockSensors[1].locked = (lockSensors[1].acVal > lockSensors[1].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
  
  if (lockSensors[2].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_3_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[2].acVal = MCP3008_getSampleSingle(LOCK_SENS_3_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_3_LIGHT, 0);
    lockSensors[2].locked = (lockSensors[2].acVal > lockSensors[2].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
  
  if (lockSensors[3].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_4_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[3].acVal = MCP3008_getSampleSingle(LOCK_SENS_4_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_4_LIGHT, 0);
    lockSensors[3].locked = (lockSensors[3].acVal > lockSensors[3].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
}


uint8_t spiSend(uint8_t data)
{
  uint8_t result; 
 // SPIC.DATA = data;
  SPID.DATA = data; 
  xQueueReceive(xSpiRx, &result, 10); 
  return result;
}
uint8_t spiSendENC(uint8_t data)
{
  uint8_t result; 
  SPIC.DATA = data;
  //SPID.DATA = data; 
  xQueueReceive(xSpiRxEnc, &result, 10); 
  return result;
}

uint8_t spiSendSpinBlock(uint8_t data)
{
  //SPDR = data;
 // SPIC.DATA=data;
  //SPCR &= ~(1<<SPIE);                
  //SPIC.INTCTRL=0x00; //Disable SPI interrupt
  while(!(SPID.STATUS&(0x80))); //(SPSR&(1<<SPIF)));
  data = SPID.DATA;//SPSR;                       Clearing interrupt flag
  data = SPID.DATA; //SPDR;                      Resfing DPI buffer register
  //SPCR |= (1<<SPIE);                 //Enable SPI Interrupt
  SPID.INTCTRL|=0x01; //low level
  return data;                     
}
uint8_t spiSendSpinBlockENC(uint8_t data)
{
  //SPDR = data;
  SPIC.DATA=data;
  //SPCR &= ~(1<<SPIE);                
  SPIC.INTCTRL=0x00; //Disable SPI interrupt
  while(!(SPIC.STATUS&(0x80))); //(SPSR&(1<<SPIF)));
  data = SPIC.DATA;//SPSR;                       Clearing interrupt flag
  data = SPIC.DATA; //SPDR;                      Resfing DPI buffer register
  //SPCR |= (1<<SPIE);                 //Enable SPI Interrupt
  SPIC.INTCTRL|=0x01; //low level
  return data;                     
}
//tu duzo zmienic i hardwareconfig.h
void disableAllSpiDevices(void)
{ 
#if disableSpiPORTA_OR !=0
  PORTA.OUT |= disableSpiPORTA_OR;
#endif
#if disableSpiPORTB_AND !=0xFF
  PORTA.OUT &= disableSpiPORTB_AND;
#endif
  
#if disableSpiPORTB_OR != 0
  PORTB.OUT |= disableSpiPORTB_OR;
#endif
#if disableSpiPORTB_AND != 0xFF 
  PORTB.OUT &= disableSpiPORTB_AND;
#endif
  
#if disableSpiPORTC_OR !=0
  PORTC.OUT |= disableSpiPORTC_OR;
#endif
#if disableSpiPORTC_AND !=0xFF
  PORTC.OUT |= disableSpiPORTC_AND;
#endif
  
#if disableSpiPORTD_OR != 0
  PORTD.OUT |= disableSpiPORTD_OR;
#endif
#if disableSpiPORTD_AND != 0xFF
  PORTD.OUT &= disableSpiPORTD_AND;
#endif
  
#if disableSpiPORTE_OR != 0
  PORTE.OUT |= disableSpiPORTE_OR;
#endif
#if disableSpiPORTE_AND != 0xFF
  PORTE.OUT &= disableSpiPORTE_AND;
#endif

#if disableSpiPORTF_OR != 0
  PORTF.OUT |= disableSpiPORTF_OR;
#endif
#if disableSpiPORTF_AND != 0xFF
  PORTF.OUT &= disableSpiPORTF_AND;
#endif

  #if disableSpiPORTJ_OR != 0
#error Port J is memory bus
  PORTJ.OUT |= disableSpiPORTJ_OR;
#endif
#if disableSpiPORTJ_AND != 0xFF
#error Port J is memory bus
  PORTJ.OUT &= disableSpiPORTJ_AND;
#endif
  
#if disableSpiPORTK_OR != 0
#error Port K is memory bus
  PORTK.OUT |= disableSpiPORTK_OR;
#endif
#if disableSpiPORTK_AND != 0xFF
#error Port K is memory bus
  PORTK.OUT &= disableSpiPORTK_AND;
#endif
  
  #if disableSpiPORTH_OR != 0
#error Port H is memory bus
  PORTK.OUT |= disableSpiPORTG_OR;
#endif
#if disableSpiPORTH_AND != 0xFF
#error Port H is memory bus
  PORTH.OUT &= disableSpiPORTG_AND;
#endif

}

void spiEnableEnc28j60(void)
{
#if ENC_SPI_CS_EN_MASK_OR != 0
  ENC_SPI_CS_PORT |= ENC_SPI_CS_EN_MASK_OR;
#endif
#if ENC_SPI_CS_EN_MASK_AND != 0xFF
  ENC_SPI_CS_PORT &= ENC_SPI_CS_EN_MASK_AND;
#endif
}

void spiDisableEnc28j60(void)
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

#define MCP3008_SPCR_OR_MASK  0b00000011
//((1<<SPR1)|(1<<SPR0))
void enableSpiMCP3008(void)
{
  SPID.CTRL|= MCP3008_SPCR_OR_MASK;
#if MCP3008_SPI_CS_EN_MASK_OR != 0
  MCP3008_SPI_CS_PORT |= MCP3008_SPI_CS_EN_MASK_OR;
#endif
#if MCP3008_SPI_CS_EN_MASK_AND != 0xFF
  MCP3008_SPI_CS_PORT &= MCP3008_SPI_CS_EN_MASK_AND;
#endif  

}

void disableSpiMCP3008(void)
{
  //SPCR
  SPID.CTRL&= ~MCP3008_SPCR_OR_MASK;
  #if MCP3008_SPI_CS_EN_MASK_OR != 0
  MCP3008_SPI_CS_PORT &= (~MCP3008_SPI_CS_EN_MASK_OR);
#endif
#if MCP3008_SPI_CS_EN_MASK_AND != 0xFF
  MCP3008_SPI_CS_PORT |= (~MCP3008_SPI_CS_EN_MASK_AND);
#endif
}


#define MCP4150_SPCR_OR_MASK  0b00000011
//((1<<SPR1)|(1<<SPR0))
void enableSpiMCP4150(void)
{
 // SPCR
 SPID.CTRL|= MCP4150_SPCR_OR_MASK;
#if MCP4150_SPI_CS_EN_MASK_OR != 0
  MCP4150_SPI_CS_PORT |= MCP4150_SPI_CS_EN_MASK_OR;
#endif
#if MCP4150_SPI_CS_EN_MASK_AND != 0xFF
  MCP4150_SPI_CS_PORT &= MCP4150_SPI_CS_EN_MASK_AND;
#endif  
}
void disableSpiMCP4150(void) 
{
  //SPCR
  SPID.CTRL&= ~MCP4150_SPCR_OR_MASK;
  #if MCP4150_SPI_CS_EN_MASK_OR != 0
  MCP4150_SPI_CS_PORT &= (~MCP4150_SPI_CS_EN_MASK_OR);
#endif
#if MCP4150_SPI_CS_EN_MASK_AND != 0xFF
  MCP4150_SPI_CS_PORT |= (~MCP4150_SPI_CS_EN_MASK_AND);
#endif  
}

#define DS_SPCR_OR_MASK 0b00000101
//((1<<CPHA)|(1<<SPR0))

void spiEnableDS1305(void)
{
  //SPCR 
  SPID.CTRL|= DS_SPCR_OR_MASK;
#if DS1305_SPI_CS_EN_MASK_OR != 0
  DS1305_SPI_CS_PORT |= DS1305_SPI_CS_EN_MASK_OR;
#endif
#if DS1305_SPI_CS_EN_MASK_AND != 0xFF
  DS1305_SPI_CS_PORT &= DS1305_SPI_CS_EN_MASK_AND;
#endif
}

void spiDisableDS1305(void)
{
  //SPCR
  SPID.CTRL &= (~(DS_SPCR_OR_MASK));
#if DS1305_SPI_CS_EN_MASK_OR != 0
  DS1305_SPI_CS_PORT &= (~(DS1305_SPI_CS_EN_MASK_OR));
#endif
#if DS1305_SPI_CS_EN_MASK_AND != 0xFF
  DS1305_SPI_CS_PORT |= (~(DS1305_SPI_CS_EN_MASK_AND));
#endif  
}

ISR(SPID_INT_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 

  static uint8_t data;
  data = SPID.DATA;//SPDR;
  
  xQueueSendFromISR(xSpiRx, &data, &xHigherPriorityTaskWoken);

  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
  
  //clear SPI interrupt SPI |= 1;
}
ISR(SPIC_INT_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 

  static uint8_t data;
  data = SPIC.DATA;//SPDR;
  
  xQueueSendFromISR(xSpiRxEnc, &data, &xHigherPriorityTaskWoken);

  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
  
  //clear SPI interrupt SPI |= 1;
}
