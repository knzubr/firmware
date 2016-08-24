#include "hardware.h"


//xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
//xQueueHandle      xSpiRxEnc;

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
  // 0 I2C              // 4 PWMA
  // 1 I2C              // 5 PWMB
  // 2 UART VTY RxD     // 6 UART radio RxD
  // 3 UART VTY TxD     // 7 UART radio TxD

  PORTC.REMAP=((1<<PORT_TC0A_bp)|(1<<PORT_TC0B_bp)) ;
  PORTC.DIR=0xB8;
  PORTC.OUT=0x00;

  ///PORT D
  // 0 Radio set/reset  4 Sim900 PWR key
  // 1 Sim900 reset     5 SPI 1
  // 2 Sim900 RxD       6 SPI 2
  // 3 Sim900 TxD       7 SPI 3
  PORTD.DIR=0x35;
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
  PORTR.OUT=0x01;


  //Timer 0 H bridge PWM
  TCC0.PER   = 100;   //Zakres
  TCC0.CTRLA = 0x07;  //Preskaler 1024
  TCC0.CTRLB = 0x33;  //A and B channels enabled, single slope
  TCC0.CTRLC = 0x00;



  TCC0.CCAH = 0x00;
  TCC0.CCBH = 0x00;
  //PORTC.OUTSET = 0x30;
}

void offHbridge()
{
    PORTR.OUTCLR = 0x02;

    PORTA.OUTCLR = 0xF0;
}

void forwardA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0x60;
    PORTA.OUTSET = 0x90;

    TCC0.CCA = left;
    TCC0.CCB = right;
}


void backwordA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0x90;
    PORTA.OUTSET = 0x60;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateLeftA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0xA0;
    PORTA.OUTSET = 0x50;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateRightA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;

    PORTA.OUTCLR = 0x50;
    PORTA.OUTSET = 0xA0;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void forwardB(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;

    PORTA.OUTCLR = 0x90;
    PORTA.OUTSET = 0x60;

    TCC0.CCA = left;
    TCC0.CCB = right;
}


void backwordB(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;

    PORTA.OUTCLR = 0x60;
    PORTA.OUTSET = 0x90;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateLeftB(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0xA0;
    PORTA.OUTSET = 0x50;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateRightB(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;

    PORTA.OUTCLR = 0x50;
    PORTA.OUTSET = 0xA0;

    TCC0.CCA = left;
    TCC0.CCB = right;
}
