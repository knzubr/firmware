#ifndef VTY_H
#define VTY_H

#include "main.h"
#include <stdio.h>
#include <util/crc16.h>
#include "ds1305.h"
#include "enc28j60.h"

// Znaki kontrolne w protokole Xmodem
#define SOH                     0x01
#define STX                     0x02
#define EOT                     0x04
#define ACK                     0x06
#define NAK                     0x15
#define CAN                     0x18
#define CTRLZ                   0x1A

// xmodem timeout/retry parameters
#define XMODEM_RETRY_LIMIT      16

// error return codes
#define XMODEM_ERROR_REMOTECANCEL       1
#define XMODEM_ERROR_OUTOFSYNC          2
#define XMODEM_ERROR_RETRYEXCEED        3

#define XMODEM_BUFFER_SIZE		128



void VtyInit(void);

extern xQueueHandle           xVtyRec;
extern xQueueHandle           xVtyTx;

extern xQueueHandle           xRs485Rec;
extern xQueueHandle           xRs485Tx;

extern volatile timeDecoded_t czasRtc;
extern struct Enc28j60_config Enc28j60_global;

#endif
