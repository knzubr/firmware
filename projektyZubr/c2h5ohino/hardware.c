#include "hardware.h"


void hardwareInit(void)
{
  /// A/C
  PRR   &= ~(1<<PRADC);
  ADMUX  = 0x40;

  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1); //Napięcie ref 5V, dzielnik 64
  //ADCSRB;
  DIDR0  = 0x01;

  /// PORTS
  DDRB  = 0x20;
  PORTB = 0x20;

  DDRC  = 0x00;
  PORTC = 0x00;

  PORTD = 0x04;  //PD0 RX,           PD1 -TX,        PD2 przycisk start
  DDRD  = 0x12;  //PD4 led RDY
}

void adcSignle(uint8_t chanNo)
{
  ADMUX  &= 0xF0;
  chanNo &= 0x0F;
  ADMUX  |= chanNo;

  ADCSRA |= (1<<ADSC);
}

uint16_t adcRead()
{
  while(ADCSRA & (1<<ADSC)); //czeka na zakończenie konwersji

  return ADC;
}
