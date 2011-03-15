#include "hardware.h"


void hardwareInit(void)
{
  DDRB  = 0x07; //0 - P3  , 1 - P2  , 2 - P1
  PORTB = 0x38; //3 - Adr3, 4 - Adr4, 5 - Adr5
  DDRC  = 0x00; //0 - Adr0, 1 - Adr1, 2 - Klawisz 1, 3 - klawisz 2
  PORTC = 0x3F; //4 - Klawisz 3, 5 - Klawisz 4
  DDRD  = 0xEE; //0 - RXD, 1 - TXD, 2 - !RxEn, 3 - TxEn, 
  PORTD = 0x00; //5 - Led1, 6 - Led2, 7 - P4
  

  /*Ustalanie adresu
    bit 7, 6 = 0 dla sterowników rolet i światła
   */
  //adres =  (PINB & 0x38) >> 1;
  //adres |= (PINC & 0x03);
}

inline void zapal1(void)       { PORTB |=  0x04;     }
inline void zgas1 (void)       { PORTB &=  ~0x04;    }

inline void zapal2(void)       { PORTB |=  0x02;     }
inline void zgas2 (void)       { PORTB &=  ~0x02;    }

inline void zapal3(void)       { PORTB |=  0x01;     }
inline void zgas3 (void)       { PORTB &=  ~0x01;    }

inline void zapal4(void)       { PORTD |=  0x80;     }
inline void zgas4 (void)       { PORTD &=  ~0x80;    }

inline void Led1On(void)       { PORTD |= 0x20;      }
inline void Led1Toggle(void)   { PORTD ^= 0x20;      }
inline void Led1Off(void)      { PORTD &= ~0x20;     }

inline void Led2On(void)       { PORTD |= 0x40;      }
inline void Led2Toggle(void)   { PORTD ^= 0x40;      }
inline void Led2Off(void)      { PORTD &= ~0x40;     }

inline char czytKlawisz1(void) { return PINC & 0x04; }
inline char czytKlawisz2(void) { return PINC & 0x08; }
inline char czytKlawisz3(void) { return PINC & 0x10; }
inline char czytKlawisz4(void) { return PINC & 0x20; }

void zapal(int nr)
{
  switch(nr)
  {
    case 0: zapal1(); break;
    case 1: zapal2(); break;
    case 2: zapal3(); break;
    case 3: zapal4(); break;
    default: break;
  }
}

void zgas(int nr)
{
  switch(nr)
  {
    case 0: zgas1(); break;
    case 1: zgas2(); break;
    case 2: zgas3(); break;
    case 3: zgas4(); break;
    default: break;
  }
}

char czytajKlawisz(int nr)
{
  char wynik;
  switch (nr)
  {
    case 0: wynik = czytKlawisz1(); break;
    case 1: wynik = czytKlawisz2(); break;
    case 2: wynik = czytKlawisz3(); break;
    case 3: wynik = czytKlawisz4(); break;
  } 
  return wynik;
}
