#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"
#include "spi.h"
#include "semphr.h"


void spiInit(void (*disableAllSpiDevicesFun)(void))
{
  disableAllSpiDevicesFun();
  portENTER_CRITICAL();
  vSemaphoreCreateBinary(xSemaphoreSpiSS); 
  xSpiRx          = xQueueCreate(1, 1);

  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPIE);
  //SPCR = (1<<SPE)|(1<<MSTR);
  SPSR |= (1<<SPI2X);
  portEXIT_CRITICAL();
  
  //mode 0,0
}

void    setSpiMode(uint8_t mode)
{
  
}

void    setSpiSpeed(uint8_t speed)
{
  
}

void spiTake(void)
{
  xSemaphoreTake(xSemaphoreSpiSS, portMAX_DELAY);
}

void spiGive(void)
{
  xSemaphoreGive(xSemaphoreSpiSS);
}


uint8_t spiSend(uint8_t data)
{
  uint8_t result;
  SPDR = data;
  xQueueReceive(xSpiRx, &result, 10); 
  return result;
}

uint8_t spiSendSpinBlock(uint8_t data)
{
  SPDR = data;
  SPCR &= ~(1<<SPIE);                //Disable SPI interrupt
  while(!(SPSR&(1<<SPIF)));
  data = SPSR;                       //Clearing interrupt flag
  data = SPDR;                       //Resfing DPI buffer register
  SPCR |= (1<<SPIE);                 //Enable SPI Interrupt
  return data;                     
}

ISR(SPI_STC_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 

  static uint8_t data;
  data = SPDR;
  
  xQueueSendFromISR(xSpiRx, &data, &xHigherPriorityTaskWoken);

  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
  
  //clear SPI interrupt SPI |= 1;
}
