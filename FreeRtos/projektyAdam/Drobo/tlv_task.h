#ifndef TLV_TASK_H
#define TLV_TASK_H

#include <stdio.h>
#include <avr/pgmspace.h>


#include "FreeRTOS.h"
#include "queue.h"
#include "cmdline.h"

#include "task.h"

#include "../../Lib/include/tlvProt.h"
#include "main.h"




extern xSemaphoreHandle HC21semaphore;

extern xQueueHandle         xTlvRec;
extern xQueueHandle         xTlvTx;

/**
 * Proces odpowiedzialny za obsługę protokołu TLV
 * Kod zoptymalizowany do szczególnego przypadku
 * @param *TlvPtr wskaźnik do struktury przechowującej stan sesji protokołu TLV
 */
void vTaskTLV(void *tlvIntPtr);





#endif /* TLV_TASK_H */
