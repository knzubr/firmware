#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"
#include "spi.h"
#include "main.h"
#include "semphr.h"

void spiInit(void)
{
  
  portENTER_CRITICAL();
  {    
    vSemaphoreCreateBinary(xSemaphoreSpiSS); 
    xSpiRx          = xQueueCreate(1, 1);
  }
  //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPIE);
  SPCR = (1<<SPE)|(1<<MSTR);
  SPSR |= (1<<SPI2X);
  portEXIT_CRITICAL();
  
  
  
  //Enc. SPI works in mode(0,0)
  //SCK be at Idle in a low state, selectable clock polarity is not supported.
  //Commands and data are sent to the device via the SI pin, with data being clocked in on the rising edge of SCK.
  //Data is driven out by the ENC28J60 on the SO line, on the falling edge of SCK.
  //The CS pin must be held low while any operation is performed and returned high when finished

}

void disableAllDevices(void)
{
#if SS_ON_PORTD_MASK == 0xFF
//  PORTD = DISABLE_SS_ON_PORTD;
#else
 #if SS_ON_PORTD_MASK !=  DISABLE_SS_ON_PORTD
//  PORTD &= ~SS_ON_PORTD_MASK;
 #endif
//  PORTD |= DISABLE_SS_ON_PORTD;
#endif

#if SS_ON_PORTE_MASK !=  DISABLE_SS_ON_PORTE
  PORTE &= ~SS_ON_PORTE_MASK;
#endif
//  PORTD |= DISABLE_SS_ON_PORTE;

#if SS_ON_PORTG_MASK !=  DISABLE_SS_ON_PORTG
  PORTG &= ~SS_ON_PORTG_MASK;
#endif
  PORTG |= DISABLE_SS_ON_PORTG;
  
  xSemaphoreGive(xSemaphoreSpiSS);
}

void enableExternalSpiDevice(uint8_t devices)
{
  xSemaphoreTake(xSemaphoreSpiSS, portMAX_DELAY);
#if SS_ON_PORTD_MASK == 0xFF
//  PORTD = devices;
#else
//  PORTD = (devices & SS_ON_PORTD_MASK);
#endif
}

void enableEnc(void)
{
  xSemaphoreTake(xSemaphoreSpiSS, portMAX_DELAY);
  PORTE &= ~0x08;
}

void enableSD(void)
{
  xSemaphoreTake(xSemaphoreSpiSS, portMAX_DELAY);
  PORTG &= ~0x08;
}

uint8_t spiSend(uint8_t data)
{
  uint8_t result;
  SPDR = data;
  xQueueReceive(xSpiRx, &result, 10); 
  return result;
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

ISR(INT7_vect)
{
  
  
}
