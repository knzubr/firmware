#include "loop_task.h"


void vTaskLoop(void *arg)
{
  (void) arg;
  uint8_t timeOfPressed = 0;
  uint8_t ctr=0;
  for( ;; )
  {
    if (!(READ2))
    {
      timeOfPressed++;
      if (timeOfPressed == 3)
      {
        PORTD |= 0x20;   //Start heating sensor TODO modify hardware
        for (ctr=0; ctr<30; ctr++)
        {
            vTaskDelay(20);
            PORTD |= 0x10;
            vTaskDelay(20);
            PORTD &= ~0x10;
        }
        PORTD &= ~0x20;  //Stop heating sensor TODO modify hardware
        adcSignle(0);
        adcRead();
        //pfrintf()
        // Print the results
      }
      if (timeOfPressed == 255)
        timeOfPressed = 254;
    }
    else
      timeOfPressed = 0;

    ctr++;
    if (ctr == 10)
        PORTB &= ~0x20;
    if (ctr == 20)
    {
      PORTB |= 0x20;
      ctr = 0;
    }
    vTaskDelay(10);
  }
}
