#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

/* Constants for writing to UCSRB. */
#define serRX_INT_ENABLE        ( ( unsigned portCHAR ) 0x80 )
#define serTX_INT_ENABLE        ( ( unsigned portCHAR ) 0x40 )
#define serDATA_INT_ENABLE      ( ( unsigned portCHAR ) 0x20 )


#define serRX_ENABLE            ( ( unsigned portCHAR ) 0x10 )
#define serTX_ENABLE            ( ( unsigned portCHAR ) 0x08 )

/* Constants for writing to UCSRC. */
#define serUCSRC_SELECT         ( ( unsigned portCHAR ) 0x80 )
#define serEIGHT_DATA_BITS      ( ( unsigned portCHAR ) 0x06 )

/// Serial 0

/**
 * Włączenie przerwania pusty bufor nadawczy dla magistrali Rs485
 */
#define vInterruptRs485On()        \
{                                  \
  unsigned portCHAR ucByte;        \
                                   \
  ucByte = UCSR0B;                 \
  ucByte |= serDATA_INT_ENABLE;    \
  UCSR0B = ucByte;                 \
}

#define vIsInterruptRs485On()  (UCSR0B & serDATA_INT_ENABLE)

/**
 * Wyłączenie przerwania pusty bufor nadawczy dla magistrali Rs485
 */
#define vInterruptRs485Off()       \
{                                  \
  unsigned portCHAR ucInByte;      \
                                   \
  ucInByte = UCSR0B;               \
  ucInByte &= ~serDATA_INT_ENABLE; \
  UCSR0B = ucInByte;               \
}


///Serial 1

/**
 * Włączenie przerwania pusty bufor nadawczy dla VTY
 */
#define vInterruptVtyOn()             \
{                                  \
  unsigned portCHAR ucByte;        \
                                   \
  ucByte = UCSR1B;                 \
  ucByte |= serDATA_INT_ENABLE;    \
  UCSR1B = ucByte;                 \
}

#define vIsInterruptVtyOn()  (UCSR1B & serDATA_INT_ENABLE)

/**
 * Wyłączenie przerwania pusty bufor nadawczy dla VTY
 */
#define vInterruptVtyOff()            \
{                                  \
  unsigned portCHAR ucInByte;      \
                                   \
  ucInByte = UCSR1B;               \
  ucInByte &= ~serDATA_INT_ENABLE; \
  UCSR1B = ucInByte;               \
}


extern xQueueHandle         xVtyRec;
extern xQueueHandle         xRs485Rec;
extern xQueueHandle         xVtyTx;
extern xQueueHandle         xRs485Tx;


int     VtyPutChar(char c, FILE *stream);

void    xSerialPortInitMinimal(void);
void    uartVtySendByte(uint8_t data);
void    uartRs485SendByte(uint8_t data);

#endif

