#include "tlv_task.h"

void vTaskTLV(void *tlvIntPtr)
{
  tlvInterpreter_t *state = (tlvInterpreter_t *)(tlvIntPtr);

  char znak;
  for( ;; )
  {
    vTaskDelay(0);
    if (xSemaphoreTake(Hc12semaphore, 5) == pdFALSE)
      continue;

    if( xQueueReceive(xHC12Rec, &znak, 0) == pdFALSE)
    {
      xSemaphoreGive(Hc12semaphore);
      continue;
    }

    HC12setTransparentMode();
    do
    {
      fprintf_P(state->errStr, PSTR("0x%02x "), znak);
      tlvProcessDta(state, znak);
    }
    while( xQueueReceive(xHC12Rec, &znak, 1) == pdTRUE);

    xSemaphoreGive(Hc12semaphore);
  }
}

