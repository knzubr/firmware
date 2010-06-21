#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "main.h"
#include "serial.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

#define debug 1

/*-----------------------------------------------------------*/

int VtyPutChar(char c, FILE *stream)
{
  stream = NULL;
//  if (c == '\n')
//    VtyPutChar('\r', stream);
  uartVtySendByte(c);
  return 0;
}

void xSerialPortInitMinimal(void)
{
  portENTER_CRITICAL();
  {
    xVtyRec = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xVtyTx = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xRs485Rec = xQueueCreate( 16, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    xRs485Tx = xQueueCreate( 4, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
  }
  portEXIT_CRITICAL();
  
  UBRR0L = 7;
  UBRR0H = 0;

  UBRR1L = 7;
  UBRR1H = 0;

  UCSR0B = ((1<<TXCIE0)|(1<<RXCIE0)|(1<<TXEN0)|(1<<RXEN0));
  UCSR0C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );     /* Set the data bits to 8. */
  UCSR1B = ((1<<RXCIE1)|(1<<TXEN1)|(1<<RXEN1));
  UCSR1C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );     /* Set the data bits to 8. */
  return;
}

/*-----------------------------------------------------------*/
ISR(USART0_RX_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken = pdTRUE; 
  signed portCHAR cChar;

  cChar = UDR0;

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

ISR(USART0_UDRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  static char data;
  if(xQueueReceiveFromISR(xRs485Tx, (void *)(&data), &xHigherPriorityTaskWoken) == pdTRUE)
  {
    Rs485TxStart();
    UDR0 = data; 
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

ISR(USART0_TX_vect)
{
  if (!vIsInterruptRs485On())
    Rs485TxStop();
}

/*-----------------------------------------------------------*/
ISR(USART1_RX_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  signed portCHAR cChar;

  cChar = UDR1;
//  xQueueSendFromISR(xVtyRec, &cChar, NULL);

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

ISR(USART1_UDRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  static char data;
  if(xQueueReceiveFromISR(xVtyTx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    UDR1 = data; 
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

