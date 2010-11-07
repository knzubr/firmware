#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// --------------------- Configuration I/O module --------------------------------

#define LOCK_SENS_1_LIGHT 0x40
#define LOCK_SENS_1_AC_IN 4

#define LOCK_SENS_2_LIGHT 0x20
#define LOCK_SENS_2_AC_IN 5

#define LOCK_SENS_3_LIGHT 0x10
#define LOCK_SENS_3_AC_IN 6

#define LOCK_SENS_4_LIGHT 0x08
#define LOCK_SENS_4_AC_IN 7


// --------------------- Konfiguracja pamięci ------------------------------------
//

#define HEAP_BEGIN                   0x1100
#define HEAP_END                     CLI_1_BUF_ADDR - 1
#define HEAP_SIZE                    HEAP_END - HEAP_BEGIN + 1

#define CLI_BUF_TOT_LEN              0x0100
#define CLI_1_BUF_ADDR               0x2800
#define CLI_2_BUF_ADDR               0x2900
#define CLI_3_BUF_ADDR               0x2A00
#define CLI_4_BUF_ADDR               0x2B00

#define ETH_SOCK_BUF_LEN             0x0200
#define ETH_SOCK_TX_1_BUF_ADDR       0x2C00
#define ETH_SOCK_RX_1_BUF_ADDR       0x2E00
#define ETH_SOCK_TX_2_BUF_ADDR       0x3000
#define ETH_SOCK_RX_2_BUF_ADDR       0x3200
#define ETH_SOCK_TX_3_BUF_ADDR       0x3400
#define ETH_SOCK_RX_3_BUF_ADDR       0x3600
#define ETH_SOCK_TX_4_BUF_ADDR       0x3800
#define ETH_SOCK_RX_4_BUF_ADDR       0x3A00
#define ETH_SOCK_TX_5_BUF_ADDR       0x3C00
#define ETH_SOCK_RX_5_BUF_ADDR       0x3E00
#define ETH_SOCK_TX_6_BUF_ADDR       0x4000
#define ETH_SOCK_RX_6_BUF_ADDR       0x4200
#define ETH_SOCK_TX_7_BUF_ADDR       0x4400
#define ETH_SOCK_RX_7_BUF_ADDR       0x4600
#define ETH_SOCK_TX_8_BUF_ADDR       0x4800
#define ETH_SOCK_RX_8_BUF_ADDR       0x4A00
#define ETH_SOCK_TX_9_BUF_ADDR       0x4C00
#define ETH_SOCK_RX_9_BUF_ADDR       0x4E00
#define ETH_SOCK_TX_10_BUF_ADDR      0x5000
#define ETH_SOCK_RX_10_BUF_ADDR      0x5200
#define ETH_SOCK_TX_11_BUF_ADDR      0x5400
#define ETH_SOCK_RX_11_BUF_ADDR      0x5600
#define ETH_SOCK_TX_12_BUF_ADDR      0x5800
#define ETH_SOCK_RX_12_BUF_ADDR      0x5A00
#define ETH_SOCK_TX_13_BUF_ADDR      0x5C00
#define ETH_SOCK_RX_13_BUF_ADDR      0x5E00
#define ETH_SOCK_TX_14_BUF_ADDR      0x6000
#define ETH_SOCK_RX_14_BUF_ADDR      0x6200
#define ETH_SOCK_TX_15_BUF_ADDR      0x6400
#define ETH_SOCK_RX_15_BUF_ADDR      0x6600
#define ETH_SOCK_TX_16_BUF_ADDR      0x6800
#define ETH_SOCK_RX_16_BUF_ADDR      0x6A00
#define ETH_SOCK_TX_17_BUF_ADDR      0x6C00
#define ETH_SOCK_RX_17_BUF_ADDR      0x6E00
#define ETH_SOCK_TX_18_BUF_ADDR      0x7000
#define ETH_SOCK_RX_18_BUF_ADDR      0x7200
#define ETH_SOCK_TX_19_BUF_ADDR      0x7400
#define ETH_SOCK_RX_19_BUF_ADDR      0x7600
#define ETH_SOCK_TX_20_BUF_ADDR      0x7800
#define ETH_SOCK_RX_20_BUF_ADDR      0x7A00

#define ENC28J60BUF_SIZE             0x0400 // 1 kB
#define NETWORK_STACK_BUF_SIZE       0x0400 // 1 kB
#define ENC28J60BUF_ADDR             0x7C00 // 31 - 32 kB
#define NETWORK_STACK_BUF            0x7C00 // 31 - 32 kB
#define ENC28J60BUF_ADDR_END         ENC28J60BUF_ADDR + ENC28J60BUF_SIZE - 1

/* Memory Map
 0x0000 +-----------------------------+
  256   | Controll registers          |
 0x0100 +-----------------------------+
  4k    | Internal memory             |
 0x1FFF +-----------------------------+
 0x1100 +-----------------------------+
  5k768 | Heap                        |
 0x2800 +-----------------------------+
    256 * CLI 1 buffer                +
 0x2900 +-----------------------------+
    256 * CLI 2 buffer                +
 0x2A00 +-----------------------------+
    256 + CLI 3 buffer                +
 0x2B00 +-----------------------------+
    256 * CLI 4 buffer                +
 0x2C00 +-----------------------------+
  512   | Socket 1  Tx buffer         |
 0x2E00 +-----------------------------+
  512   | Socket 1  Rx buffer         |
 0x3000 +-----------------------------+
  512   | Socket 2  Tx buffer         |
 0x3200 +-----------------------------+
  512   | Socket 2  Rx buffer         |
 0x3400 +-----------------------------+
  512   | Socket 3  Tx buffer         |
 0x3600 +-----------------------------+
  512   | Socket 3  Rx buffer         |
 0x3800 +-----------------------------+
  512   | Socket 4  Tx buffer         |
 0x3A00 +-----------------------------+
  512   | Socket 4  Rx buffer         |
 0x3C00 +-----------------------------+
  512   | Socket 5  Tx buffer         |
 0x3E00 +-----------------------------+
  512   | Socket 5  Rx buffer         |
 0x4000 +-----------------------------+
  512   | Socket 6  Tx buffer         |
 0x4200 +-----------------------------+
  512   | Socket 6  Rx buffer         |
 0x4400 +-----------------------------+
  512   | Socket 7  Tx buffer         |
 0x4600 +-----------------------------+
  512   | Socket 7  Rx buffer         |
 0x4800 +-----------------------------+
  512   | Socket 8  Tx buffer         |
 0x4A00 +-----------------------------+
  512   | Socket 8  Rx buffer         |
 0x4C00 +-----------------------------+
  512   | Socket 9  Tx buffer         |
 0x4E00 +-----------------------------+
  512   | Socket 9  Rx buffer         |
 0x5000 +-----------------------------+
  512   | Socket 10 Tx buffer         |
 0x5200 +-----------------------------+
  512   | Socket 10 Rx buffer         |
 0x5400 +-----------------------------+
  512   | Socket 11 Tx buffer         |
 0x5600 +-----------------------------+
  512   | Socket 11 Rx buffer         |
 0x5800 +-----------------------------+
  512   | Socket 12 Tx buffer         |
 0x5A00 +-----------------------------+
  512   | Socket 12 Rx buffer         |
 0x5C00 +-----------------------------+
  512   | Socket 13 Tx buffer         |
 0x5E00 +-----------------------------+
  512   | Socket 13 Rx buffer         |
 0x6000 +-----------------------------+
  512   | Socket 14 Tx buffer         |
 0x6200 +-----------------------------+
  512   | Socket 14 Rx buffer         |
 0x6400 +-----------------------------+
  512   | Socket 15 Tx buffer         |
 0x6600 +-----------------------------+
  512   | Socket 15 Rx buffer         |
 0x6800 +-----------------------------+
  512   | Socket 16 Tx buffer         |
 0x6A00 +-----------------------------+
  512   | Socket 16 Rx buffer         |
 0x6C00 +-----------------------------+
  512   | Socket 17 Tx buffer         |
 0x6E00 +-----------------------------+
  512   | Socket 17 Rx buffer         |
 0x7000 +-----------------------------+
  512   | Socket 18 Tx buffer         |
 0x7200 +-----------------------------+
  512   | Socket 18 Rx buffer         |
 0x7400 +-----------------------------+
  512   | Socket 19 Tx buffer         |
 0x7600 +-----------------------------+
  512   | Socket 19 Rx buffer         |
 0x7800 +-----------------------------+
  512   | Socket 20 Tx buffer         |
 0x7A   +-----------------------------+
  512   | Socket 20 Rx buffer         |
 0x7C00 +-----------------------------+
  1k    | Enc28j60Buffer              |
 0x8000 +-----------------------------+
  32k   | Filesystem Fat8             |
 0xFFFF +-----------------------------+
*/


//Konfiguracja biblioteki ds1305.h
#define USE_DECODED_TIME_STRUCT 1

//Konfiguracja Sterownika ethenretowego Enc28j60
//CS jest na PORT E.3
#define ENC_SPI_CS_PORT              PORTE
#define ENC_SPI_CS_EN_MASK_OR        0x00
#define ENC_SPI_CS_EN_MASK_AND       0xF7

//Konfiguracja Karty SD
//CS jest na PORT G.3
#define SD_SPI_CS_PORT               PORTG
#define SD_SPI_CS_EN_MASK_OR         0x00
#define SD_SPI_CS_EN_MASK_AND        0xF7

//Konfiguracja portu równoległego MPC23S17
//CS jest na PORT B.7
#define MPC23S17_SPI_CS_PORT         PORTB
#define MPC23S17_SPI_CS_EN_MASK_OR   0x00
#define MPC23S17_SPI_CS_EN_MASK_AND  0x7F

//Konfiguracja Układu analogowo cyfrowego MPC3008
//CS jest na PORT B.6
#define MCP3008_SPI_CS_PORT          PORTB
#define MCP3008_SPI_CS_EN_MASK_OR    0x00
#define MCP3008_SPI_CS_EN_MASK_AND   0xBF

//Konfiguracja Zegara czasu rzeczywistego DS1305
//CE jest na PORT B.5
#define DS1305_SPI_CS_PORT           PORTB
#define DS1305_SPI_CS_EN_MASK_OR     0x20
#define DS1305_SPI_CS_EN_MASK_AND    0xFF



//konfiguracja wyłączania wszystkich urządzeń SPI

//PORT A: Zewnętrzna pamięć
#define disableSpiPORTA_OR           0x00
#define disableSpiPORTA_AND          0xFF

//PORT B: SPICS na PB4-PB7
// PB4 - brak
// PB5 - DS1305     0 - off, 1 - on
// PB6 - MCP3008    0 - on.  1 - off
// PB7 - MCP23S17   0 - on,  1 - off
#define disableSpiPORTB_OR           0xC0
#define disableSpiPORTB_AND          0xDF

//PORT C: Zewnętrzna pamięć
#define disableSpiPORTC_OR           0x00
#define disableSpiPORTC_AND          0xFF

//PORD D: brak SPI SS
#define disableSpiPORTD_OR           0x00
#define disableSpiPORTD_AND          0xFF

//PORT E
// PE3 - ENC28j60  0 - on,  1 - off
#define disableSpiPORTE_OR           0x08
#define disableSpiPORTE_AND          0xFF

//PORT F - brak SPI SS
#define disableSpiPORTF_OR           0x00
#define disableSpiPORTF_AND          0xFF

//PORT G
// PG3 - SD
#define disableSpiPORTG_OR           0x08
#define disableSpiPORTG_AND          0xFF

#endif