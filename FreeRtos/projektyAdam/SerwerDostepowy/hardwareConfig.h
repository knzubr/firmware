#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

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
#define disableSpiPORTG_OR           0x00
#define disableSpiPORTG_AND          0xF7

#endif