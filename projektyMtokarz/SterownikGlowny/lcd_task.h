#ifndef LCD_TASK_H
#define LCD_TASK_H
#include "FreeRTOS.h"
#include "queue.h"
#include "lcd.h"

void vTasklcd(void *cliStatePtr);

#endif
