#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "hardware.h"

#define debug 1

/*-----------------------------------------------------------*/

void initQueueStreamUSB(FILE *stream)
{
  fdev_setup_stream(stream, VtyPutChar, VtyGetChar, _FDEV_SETUP_RW);
  fdev_set_udata(stream, NULL);
  return;
}

int VtyGetChar(FILE *stream)
{
  stream = NULL;
  uint8_t c;
  if (xQueueReceive(xVtyRec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int VtyPutChar(char c, FILE *stream)
{
  stream = NULL;
  uartVtySendByte(c);
  return 0;
}

//#include<avr/iox128a1.h>
void xSerialPortInitMinimal(void)
{ 
  /// Konfiguracja portu D dla USARTD0 USB
  PORTD.DIRSET = PIN3_bm; //ok
  PORTD.DIRCLR = PIN2_bm; //ok
  PORTD.OUTSET = PIN3_bm; //ok
  
  USARTD0.CTRLA = USART_RXCINTLVL_LO_gc | USART_DREINTLVL_LO_gc;                   // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;           // Włączenie nadajnika i odbiornika
  USARTD0.CTRLC = USART_CHSIZE_8BIT_gc;                    // Tryb 8 bitów 
  // 115200 @ 32MHz
  USARTD0.BAUDCTRLA= 2094 & 0xFF;                          //12; BSEL = 131  BSCALE = -3 //USARTD0.BAUDCTRLA= 131;
  USARTD0.BAUDCTRLB= (-7 << USART_BSCALE0_bp)|(2094 >> 8); //USARTD0.BAUDCTRLB= 0xD0;// ((-3) << USART_BSCALE0_bp)|(131 >> 8);
  
  /// Konfiguracja portu F dla USARTF0 RS486
  PORTF.DIRSET = PIN3_bm; //ok
  PORTF.DIRCLR = PIN2_bm; //ok
  PORTF.OUTSET = PIN3_bm; //ok
  //USARTF0.CTRLA = USART_RXCINTLVL_LO_gc;             // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTF0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;     // Włączenie nadajnika i odbiornika
  USARTF0.CTRLC = USART_CHSIZE_8BIT_gc;              // Tryb 8 bitów 
  // 115200 @ 32MHz
  USARTF0.BAUDCTRLA= 2094 & 0xFF;                    //12; BSEL = 131  BSCALE = -3
  USARTF0.BAUDCTRLB= (-7 << USART_BSCALE0_bp)|(2094 >> 8);  
  portENTER_CRITICAL();
  {
    xVtyRec = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xVtyTx = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    //xRs485Rec = xQueueCreate( 16, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    //xRs485Tx = xQueueCreate( 4, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    
    //vSemaphoreCreateBinary(xSemaphoreRs485); 
  }
  portEXIT_CRITICAL();
  return;
}

/*-----------------------------------------------------------*/
ISR(USARTF0_RXC_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken = pdTRUE; 
  signed portCHAR cChar;

  cChar = USARTF0.DATA;//UDR0;

  xHigherPriorityTaskWoken = pdFALSE;

//  xQueueSendFromISR( xRs485Rec, &cChar, NULL);
  xQueueSendFromISR( xRs485Rec, &cChar, &xHigherPriorityTaskWoken );
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

void uartRs485SendByte(uint8_t data)
{
  xQueueSend(xRs485Tx, &data, portMAX_DELAY);
  vInterruptRs485On();
}

uint8_t rs485Receive(uint8_t *c, uint8_t timeout)
{
  return xQueueReceive(xRs485Rec, c, timeout);
}

ISR(USARTF0_DRE_vect) //   USART0_UDRE_vect
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  static char data;
  if(xQueueReceiveFromISR(xRs485Tx, (void *)(&data), &xHigherPriorityTaskWoken) == pdTRUE)
  {
    Rs485TxStart();
    USARTF0.DATA = data; 
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptRs485Off();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

ISR(USARTF0_TXC_vect) //  USART0_TX_vect
{
  if (!vIsInterruptRs485On())
    Rs485TxStop();
}

uint8_t flushRs485RecBuffer(void)
{
  uint8_t temp;
  uint8_t wynik = 0;
  while(xQueueReceive(xRs485Rec, &temp, 10) == pdTRUE)
    wynik++;
    
  return wynik;
}

void    takeRs485(void)
{
  xSemaphoreTake(xSemaphoreRs485, portMAX_DELAY);
}

void    releaseRs485(void)
{
  xSemaphoreGive(xSemaphoreRs485);
}

/*-----------------------------------------------------------*/
ISR(USARTD0_RXC_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  signed portCHAR cChar;

  cChar = USARTD0.DATA;
  
  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xVtyRec, &cChar, &xHigherPriorityTaskWoken);
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

void uartVtySendByte(uint8_t data)
{
  xQueueSend(xVtyTx, &data, portMAX_DELAY);
  vInterruptVtyOn();
}

ISR(USARTD0_DRE_vect) // USART1_UDRE_vect
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  static char data;
  if(xQueueReceiveFromISR(xVtyTx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    USARTD0.DATA = data; 
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptVtyOff();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

