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
  (void) stream;
  uint8_t c;
  if (xQueueReceive(xVtyRec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int VtyPutChar(char c, FILE *stream)
{
  (void) stream;
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

  /// F0 - port, do którego podłączono rejestrator
  PORTF.DIRSET = PIN3_bm; //ok
  PORTF.DIRCLR = PIN2_bm; //ok
  PORTF.OUTSET = PIN3_bm; //ok
  USARTF0.CTRLA = USART_RXCINTLVL_LO_gc;             // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTF0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;     // Włączenie nadajnika i odbiornika
  USARTF0.CTRLC = USART_CHSIZE_8BIT_gc;              // Tryb 8 bitów
  // 115200 @ 32MHz
  USARTF0.BAUDCTRLA= 2094 & 0xFF;                    //12; BSEL = 131  BSCALE = -3
  USARTF0.BAUDCTRLB= (-7 << USART_BSCALE0_bp)|(2094 >> 8);

  /// C1 - port, do którego podłączono kamery
  PORTC.DIRSET = PIN7_bm; //ok
  PORTC.DIRCLR = PIN6_bm; //ok
  PORTC.OUTSET = PIN7_bm; //ok
//USARTC1.CTRLA = USART_RXCINTLVL_LO_gc;             // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTC1.CTRLB = USART_RXEN_bm |  USART_TXEN_bm;    // Włączenie nadajnika i odbiornika
  USARTC1.CTRLC = USART_CHSIZE_8BIT_gc;              // Tryb 8 bitów
  // 115200 @ 32MHz
  USARTC1.BAUDCTRLA= 2094 & 0xFF;                    //12; BSEL = 131  BSCALE = -3
  USARTC1.BAUDCTRLB= (-7 << USART_BSCALE0_bp)|(2094 >> 8);


  portENTER_CRITICAL();
  {
    xVtyRec = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xVtyTx = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));

    xRs485_2_Tx  =  xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xRs485_1_Rec =  xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));

    //xRs485Rec = xQueueCreate( 16, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    //xRs485Tx = xQueueCreate( 4, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );

    //vSemaphoreCreateBinary(xSemaphoreRs485);
  }
  portEXIT_CRITICAL();
  return;
}

void uartRs485_2_SendByte(uint8_t data)
{
  xQueueSend(xRs485_2_Tx, &data, portMAX_DELAY);
  vInterruptRs485_2_On();
}


/** VTY ---------------------------------------------------*/
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


/** Obsługa porty do którego podłączone są kamery */

ISR(USARTC1_TXC_vect) //  USART0_TX_vect
{
  if (!vIsInterruptRs485On_2())
    Rs485TxStop_2();
}

ISR(USARTC1_DRE_vect) //   USART0_UDRE_vect
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xRs485_2_Tx, (void *)(&data), &xHigherPriorityTaskWoken) == pdTRUE)
  {
    Rs485TxStart_2();
    USARTC1.DATA = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptRs485_2_Off();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}


