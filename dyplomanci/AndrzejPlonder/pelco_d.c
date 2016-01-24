#include "pelco_d.h"

void pelcoInit()
{
  portENTER_CRITICAL();
  {
    pelcoMessages =  xQueueCreate(4, ( unsigned portBASE_TYPE ) sizeof( struct pelcoMsg ));
  }
  portEXIT_CRITICAL();
}


void vTaskPelco(void* pvParameters)
{
  (void) pvParameters;

  static struct pelcoMsg inMsg, outMsg;
  for( ; ; )
  {
    if( xQueueReceive(pelcoMessages, &inMsg, portMAX_DELAY))
    {
      //uint16_t crc = 0;
      //crc = _crc_xmodem_update(0, SYNC);            uartRs485SendByte(SYNC);

        uint8_t crc = 0;

        outMsg.sync=0xFF;

        outMsg.addr   = translateTable[inMsg.addr]; crc =outMsg.addr;
        outMsg.cmd[0] = inMsg.cmd[0];               crc+=outMsg.cmd[0];
        outMsg.cmd[1] = inMsg.cmd[1];               crc+=outMsg.cmd[1];
        outMsg.opt[0] = inMsg.opt[0];               crc+=outMsg.opt[0];
        outMsg.opt[1] = inMsg.opt[1];               crc+=outMsg.opt[1];
        outMsg.crc = crc;

        xQueueSend(xRs485_2_Tx, &outMsg, portMAX_DELAY);
    }
  }
}



ISR(USARTF0_RXC_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static uint8_t byteNo;
  static struct pelcoMsg tmpMsg;

  uint8_t value;

  value = USARTD0.DATA;

  xHigherPriorityTaskWoken = pdFALSE;

  if (byteNo == 0)
  {
    if (value == 0xFF)
      byteNo = 1;
    return;
  }

  uint8_t *msgRaw = (uint8_t *)(&tmpMsg);
  msgRaw[byteNo] = value;
  byteNo++;

  if (byteNo == 7)
  {
    byteNo = 0;
    xQueueSendFromISR(pelcoMessages, &tmpMsg, &xHigherPriorityTaskWoken);
  }

  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

