#include "bootloader.h"
#include "serial.h"

//send data to comport
void WriteCom(unsigned char dat)
{
  RS485Enable();
  UDRREG(COMPORTNo) = dat;
  while(!(UCSR0A & (1<<TXCBIT(COMPORTNo))))
    ;    //wait for send finish

  UCSR0A |= (1 << TXCBIT(COMPORTNo));
  RS485Disable();
}

uint8_t ReadCom_withLimitedWaiting()
{
  uint8_t cnt2 = 30;
  while(!DataInCom())
  {
    if(TIFRREG & (1<<OCF1A))    // Sprawdzanie przepełnienie timera T1
    {
      TIFRREG |= (1 << OCF1A);  // Zerowanie flagi przepełnienia

      cnt2--;                    // Odliczanie w dół. Jak dojdzie do zera to nastąpi wyjście z xModemu
      if(cnt2 == 0)              // Sprawdzanie czy nie nastąpił timeout
      {
        quit();                 // Opuszczenie bootloadera
      }
    }
  }
  return UDR0;
}


