#include "hc12.h"

void HC12setAtMode()
{
  PORTD.OUTCLR = 0x01;
}

void HC12setTransparentMode()
{
  PORTD.OUTSET = 0x01;
}

