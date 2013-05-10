#ifndef LCD_TASK_H
#define LCD_TASK_H
#include "FreeRTOS.h"
#include "queue.h"
#include "lcd.h"
#include "task.h"
extern xQueueHandle         xLCDrec;
void vTasklcd(void *cliStatePtr);

#endif
