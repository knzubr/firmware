#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// --------------------- Configuration I/O module --------------------------------

//Locker 1 (723 / 687) broken
#define LOCK_SENS_1_LIGHT 0x40
#define LOCK_SENS_1_AC_IN 4
#define LOCK_SENS_1_THR   700
#define LOCK_SENS_1_ENA   0

//Locker 2 (603 / 993) OK
#define LOCK_SENS_2_LIGHT 0x20
#define LOCK_SENS_2_AC_IN 5
#define LOCK_SENS_2_THR   750
#define LOCK_SENS_2_ENA   1

//Locker 3 (934/937) broken
#define LOCK_SENS_3_LIGHT 0x10
#define LOCK_SENS_3_AC_IN 6
#define LOCK_SENS_3_THR   700
#define LOCK_SENS_3_ENA   0

//Locker 4 (831 / 980) OK
#define LOCK_SENS_4_LIGHT 0x08
#define LOCK_SENS_4_AC_IN 7
#define LOCK_SENS_4_THR   900
#define LOCK_SENS_4_ENA   1

#define ENC_RST_ON   // PORTE &= ~0x04;
#define ENC_RST_OFF  // PORTE |= ~0x04; 


// --------------------- Konfiguracja pamięci ------------------------------------
//

#define HEAP_BEGIN                   0x004000//0x1100 // tu zmienic 0x40000UL
#define HEAP_END                     CLI_1_BUF_ADDR - 1
#define HEAP_SIZE                    HEAP_END - HEAP_BEGIN + 1

#define CLI_BUF_TOT_LEN              0x000100
#define CLI_1_BUF_ADDR               0x005700
#define CLI_2_BUF_ADDR               0x005800
#define CLI_3_BUF_ADDR               0x005900
#define CLI_4_BUF_ADDR               0x005A00
#define LCD_BUFF_ADDR		      0x005B00

#define RTOS_TCP_BUF_BASE_ADDR       0x2C00

#define FAT32_BUF_ADDR               0x7000
#define FAT32_BUF_LENGTH             0x0800


#define RTOS_UDP_TX_BUF_ADDR         0x7800
#define RTOS_UDP_RX_BUF_ADDR         0x7900

#define NETWORK_STACK_BUF_SIZE       0x0600 // 1532 bytes
#define NETWORK_STACK_BUF_ADDR       0x7A00 // 30 1/4 - 32 kB
#define ENC28J60BUF_ADDR_END         ENC28J60BUF_ADDR + ENC28J60BUF_SIZE - 1

/* Memory Map
 0x0000 +-----------------------------+
  256   | Controll registers          |
 0x0100 +-----------------------------+
  4k    | Internal memory             |
 0x1FFF +-----------------------------+  4k
 
 
 
 
 0x1100 +-----------------------------+
  5k768 | Heap                        |
 0x2800 +-----------------------------+  11 k 768
    256 * CLI 1 buffer                +
 0x2900 +-----------------------------+
    256 * CLI 2 buffer                +
 0x2A00 +-----------------------------+
           TCP buffers

 
 0x7000 +-----------------------------+  28 k
        | Fat32 Buffer                | 
 0x7800 +-----------------------------+  30 k
  256   | RTOS UDP Tx buffer          |
 0x7900 +-----------------------------+
  256   | RTOS UDP Rx buffer          |
 0x7A00 +-----------------------------+
  1k512 | Enc28j60Buffer              |
 0x8000 +-----------------------------+  32 K
  32k   | Filesystem Fat8             |
 0xFFFF +-----------------------------+
*/
/* Memory Map - new
 0x000000 +-----------------------------+ 
  256     | Controll registers          | 
 0x000100 +-----------------------------+ 
          | EEPROM                      | 
 0x001000 +-----------------------------+ 
  8k      | Internal memory             | 
 0x002000 +-----------------------------+  8k  
  
 0x004000 +-----------------------------+ 
  5k768   | Heap                        | 
 0x005700 +-----------------------------+  11 k 768
      256 * CLI 1 buffer                + 
 0x005800 +-----------------------------+ 
      256 * CLI 2 buffer                + 
 0x005900 +-----------------------------+ 
      256 * CLI 3 buffer                + 
 0x005A00 +-----------------------------+ 
      256 * CLI 4 buffer                +
 0x005B00 +-----------------------------+
      256 * LCD buffer                  +
 0x005C00 +-----------------------------+
           TCP buffers 
 
  
 0x007000 +-----------------------------+  28 k 
          | Fat32 Buffer                |  
 0x007800 +-----------------------------+  30 k 
    256   | RTOS UDP Tx buffer          | 
 0x007900 +-----------------------------+ 
    256   | RTOS UDP Rx buffer          | 
 0x007A00 +-----------------------------+ 
    1k512 | Enc28j60Buffer              | 
 0x008000 +-----------------------------+  32 K 
    32k   | Filesystem Fat8             | 
 0x00FFFF +-----------------------------+ 
*/ 
 

//Konfiguracja biblioteki ds1305.h
#define USE_DECODED_TIME_STRUCT 1
//tu zaczynam zmieniac
//Konfiguracja Sterownika ethenretowego Enc28j60
//CS jest na PORT B.0
#define ENC_SPI_CS_PORT              PORTB.OUT //B0 TU CHUBA PORTB.OUT
#define ENC_SPI_CS_EN_MASK_OR        0x00
#define ENC_SPI_CS_EN_MASK_AND       0xFE

//Konfiguracja Karty SD
//CS jest na PORT C.4
#define SD_SPI_CS_PORT               PORTC.OUT // PORTC.OUT
#define SD_SPI_CS_EN_MASK_OR         0x00
#define SD_SPI_CS_EN_MASK_AND        0xEF

//Konfiguracja portu równoległego MPC23S17
//CS jest na PORT E.7 ADR7
#define MPC23S17_SPI_CS_PORT         PORTE.OUT
#define MPC23S17_SPI_CS_EN_MASK_OR   0x00
#define MPC23S17_SPI_CS_EN_MASK_AND  0x7F

//Konfiguracja Układu analogowo cyfrowego MPC3008
//CS jest na PORT E.6
#define MCP3008_SPI_CS_PORT          PORTE.OUT
#define MCP3008_SPI_CS_EN_MASK_OR    0x00
#define MCP3008_SPI_CS_EN_MASK_AND   0xBF

//Konfiguracja rezystora cyfrowego MCP4150
//CS jest na PORT E.6
#define MCP4150_SPI_CS_PORT          PORTE.OUT
#define MCP4150_SPI_CS_EN_MASK_OR    0x00
#define MCP4150_SPI_CS_EN_MASK_AND   0xBF


//Konfiguracja Zegara czasu rzeczywistego DS1305
//CE jest na PORT E.5 E 5
#define DS1305_SPI_CS_PORT           PORTE.OUT
#define DS1305_SPI_CS_EN_MASK_OR     0x20
#define DS1305_SPI_CS_EN_MASK_AND    0xFF


//konfiguracja wyłączania wszystkich urządzeń SPI
// PORT A : brak SPI
#define disableSpiPORTA_OR	      0x00
#define disableSpiPORTA_AND          0xFF
// PORT B : ENC28j60  0 - on,  1 - off
#define disableSpiPORTB_OR           0x01
#define disableSpiPORTB_AND          0xFE
// PORT C : SPICS SD_CARD PC.4
#define disableSpiPORTC_OR           0x10
#define disableSpiPORTC_AND          0xEF
// PORT D : brak SPI SS
#define disableSpiPORTD_OR           0x00
#define disableSpiPORTD_AND          0xFF

//PORT E: SPICS na PE4-PE7 
// PE4 - brak
// PE5 - DS1305     0 - off, 1 - on
// PE6 - MCP3008    0 - on.  1 - off
// PE7 - MCP23S17   0 - on,  1 - off
#define disableSpiPORTE_OR           0xC0
#define disableSpiPORTE_AND          0xDF
//PORT F : brak SPI SS
#define disableSpiPORTF_OR           0x00
#define disableSpiPORTF_AND          0xFF

//PORT J: Zewnętrzna pamięć
#define disableSpiPORTJ_OR           0x00 //bylo portA
#define disableSpiPORTJ_AND          0xFF
//PORT K: Zewnętrzna pamięć
#define disableSpiPORTK_OR	      0x00
#define disableSpiPORTK_AND	      0xFF

//PORT H: Zewnetrzna pamiec A18-16
#define disableSpiPORTH_OR	      0x00
#define disableSpiPORTH_AND	      0xFF


#endif