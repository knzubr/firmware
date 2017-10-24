#include "hardware.h"

#if LANG_EN
#include "hardware_en.h"
#endif

#if LANG_PL
#include "hardware_pl.h"
#endif

xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu

void hardwareInit(void)
{

}

