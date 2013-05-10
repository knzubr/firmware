#include "lcd_task.h"

void vTasklcd(void *cliStatePtr)
{
    (void) cliStatePtr;;
    char znak;
    for(;;)
    { 
      if( xQueueReceive(xLCDrec, &znak, portMAX_DELAY))
      vTaskDelay(100);
      lcdWrite(znak);
    }  
}