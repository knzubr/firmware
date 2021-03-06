#ifndef CLI_TASK_H
#define CLI_TASK_H

#include <stdio.h>
#include <avr/pgmspace.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "cmdline.h"

extern xQueueHandle         xVtyRec;
extern xQueueHandle         xVtyTx;

/**
 * Proces odpowiedzialny za obsługę VTY
 * Kod zoptymalizowany do szczególnego przypadku
 * @param *cliStatePtr wskaźnik do struktury przechowującej stan interpretera poleceń
 */
void vTaskVTY(void *cliStatePtr);



#endif /* CLI_TASK_H */
