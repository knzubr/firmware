#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

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

//Konfiguracja Portu równoległego MPC23S17
//CS jest na PORT B.7
#define MPC23S17_SPI_CS_PORT         PORTB
#define MPC23S17_SPI_CS_EN_MASK_OR   0x00
#define MPC23S17_SPI_CS_EN_MASK_AND  0x7F

//Konfiguracja Zegara czasu rzeczywistego DS1305
//CE jest na PORT B.5
#define DS1305_SPI_CS_PORT           PORTB
#define DS1305_SPI_CS_EN_MASK_OR     0x20
#define DS1305_SPI_CS_EN_MASK_AND    0xFF

//Konfiguracja Układu analogowo cyfrowego MPC3008
//CS jest na PORT B.6
#define MCP3008_SPI_CS_PORT          PORTB
#define MCP3008_SPI_CS_EN_MASK_OR    0x00
#define MCP3008_SPI_CS_EN_MASK_AND   0xBF

//konfiguracja wyłączania wszystkich urządzeń SPI
#define disableSpiPORTA_OR           0x00
#define disableSpiPORTA_AND          0xFF
#define disableSpiPORTB_OR           0x00
#define disableSpiPORTB_AND          0xFF
#define disableSpiPORTC_OR           0x00
#define disableSpiPORTC_AND          0xFF
#define disableSpiPORTD_OR           0x00
#define disableSpiPORTD_AND          0xFF
#define disableSpiPORTE_OR           0x00
#define disableSpiPORTE_AND          0xFF
#define disableSpiPORTF_OR           0x00
#define disableSpiPORTF_AND          0xFF
#define disableSpiPORTG_OR           0x00
#define disableSpiPORTG_AND          0xFF

#endif