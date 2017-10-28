#include "hardware.h"

void hardwareInit(void)
{
  /// A/C
  PRR   &= ~(1<<PRADC);
  ADMUX  = 0x40;
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);

  /// PORTS
  DDRB  = 0x20;
  PORTB = 0x20;

  DDRD  = 0x02;
}

