#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"
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




// ******************************* Serial 1 USB ***********************

/*
 * Włączenie przerwania pusty bufor nadawczy dla VTY
 */


/**
 * Serial 1 (VTY) receiver que
 */
extern xQueueHandle         xVtyRec;
extern xQueueHandle         xVtyTx;

/**
 * Serial 0 (EthReplacement) receiver que
 */
extern xQueueHandle         xRsLanRec;
extern xQueueHandle         xRsLanTx;


void    xSerialPortInitMinimal(void);
void    initQueueVtyStream(FILE *stream);

int     VtyPutChar(char c, FILE *stream);
int     VtyGetChar(FILE *stream);

void    uartVtySendByte(uint8_t data);
void    uartLanSendByte(uint8_t data);
uint8_t uartLanReceiveByte(uint8_t *c);

void    InterruptVtyOn(void);
void    InterruptVtyOff(void);
#define vIsInterruptVtyOn()    (UCSR1B & serDATA_INT_ENABLE)

void    InterruptRsLanOff(void);
void    vInterruptRsLanOn(void);
#define vIsInterruptRsLanOn()  (UCSR0B & serDATA_INT_ENABLE)

#endif
