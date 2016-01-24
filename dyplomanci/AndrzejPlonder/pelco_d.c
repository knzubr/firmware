#include "pelco_d.h"



void pelcoInit()
{
  portENTER_CRITICAL();
  {
    pelcoMessages =  xQueueCreate(4, ( unsigned portBASE_TYPE ) sizeof( struct pelcoMsg ));
  }
  portEXIT_CRITICAL();
}


