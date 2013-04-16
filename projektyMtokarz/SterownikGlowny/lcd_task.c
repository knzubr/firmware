#include "lcd_task.h"

void vTasklcd(void *cliStatePtr)
{
    
    for(;;)
    {
      vTaskDelay(100);
      lcdWrite('a');
    }  
}