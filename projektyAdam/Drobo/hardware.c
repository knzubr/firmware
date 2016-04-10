#include "hardware.h"

#if LANG_EN
#include "hardware_en.h"
#endif

#if LANG_PL
#include "hardware_pl.h"
#endif

xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
xQueueHandle      xSpiRxEnc;
// napisać funkcje do konfiguracji zew pam s-ram

//#include<avr/iox128a1.h>
void hardwareInit(void)
{
  /// PORT A MOSTKI H
  //0 - AIN2_A (18) 4 - AIN2_B (+AIN2_A)
  //1 - AIN1_A (17) 5 - AIN1_B (+AIN1_A)
  //2 - BIN1_A (20) 6 - BIN1_B (+BIN1_A)
  //3 - BIN2_A (19) 7 - BIN2_B (+BIN2_A)
  PORTA.DIR=0xFF;
  PORTA.OUT=0x00;

  ///PORT B - Złącze 20 pin
  //0 - 13          2 - 14
  //1 - 14          3 - 15
  PORTB.DIR=0x00;
  PORTB.OUT=0x00;

  ///PORT C
  // 0 I2C      // 4 UART VTY RxD
  // 1 I2C      // 5 UART VTY TxD
  // 2 PWMA     // 6 UART radio RxD
  // 3 PWMB     // 7 UART radio TxD
  PORTC.DIR=0x0C;
  PORTC.OUT=0x00;

  ///PORT D
  // 0 Radio set/reset  4 Sim900 PWR key
  // 1 Sim900 reset     5 SPI 1
  // 2 Sim900 RxD       6 SPI 2
  // 3 Sim900 TxD       7 SPI 3
  PORTD.DIR=0x31;
  PORTD.OUT=0x00;

  /// PORT E
  // 0 RPI 3v3          2 RPI RxD
  // 1 RPI 5V           3 RPI TxD
  PORTE.DIR=0x0B;
  PORTD.OUT=0x00;

  /// PORT R
  // 0 SIM900 PWR HW
  // 1 MOSTEK H Stand by
  PORTR.DIR=0x03;
  PORTR.OUT=0x00;
}
