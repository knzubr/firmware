/*

	FreeRTOS.org V5.2.0 - Copyright (C) 2003-2009 Richard Barry.
	This file is part of the FreeRTOS.org distribution.
	FreeRTOS.org is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License (version 2) as published
	by the Free Software Foundation and modified by the FreeRTOS exception.
	FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA  02111-1307  USA.

	A special exception to the GPL is included to allow you to distribute a
	combined work that includes FreeRTOS.org without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details.
	***************************************************************************
	*                                                                         *
	* Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
	*                                                                         *
	* This is a concise, step by step, 'hands on' guide that describes both   *
	* general multitasking concepts and FreeRTOS specifics. It presents and   *
	* explains numerous examples that are written using the FreeRTOS API.     *
	* Full source code for all the examples is provided in an accompanying    *
	* .zip file.                                                              *
	*                                                                         *
	***************************************************************************
	1 tab == 4 spaces!
	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/

#include "main.h"


uint8_t timer100Hz = 0;

uint8_t translateTable[256];

xQueueHandle xVtyTx;
xQueueHandle xVtyRec;

xQueueHandle xSim900Rec;
xQueueHandle xSim900Tx;

xQueueHandle xRadioTx;
xQueueHandle xRadioRec;


void vApplicationIdleHook( void );

/**
 * RTC clock support
 */
void vApplicationTickHook( void );

xTaskHandle xHandleVTY_USB;

//xTaskHandle xHandleTranslator;

/**
  * konfiguracja zegara. Zegar wewnętrzny 32 MHz
  */
void my_init_clock(void)
{
  // Internal 32 kHz RC oscillator initialization
  // Enable the internal 32 kHz RC oscillator
  OSC.CTRL|=OSC_RC32KEN_bm;
  // Wait for the internal 32 kHz RC oscillator to stabilize
  while ((OSC.STATUS & OSC_RC32KRDY_bm)==0);

  // Internal 32 MHz RC oscillator initialization
  // Enable the internal 32 MHz RC oscillator
  OSC.CTRL|=OSC_RC32MEN_bm;

  // System clock prescaler A division factor: 1
  // System clock prescalers B & C division factors: B:1, C:1
  // ClkPer4: 32000.000 kHz
  // ClkPer2: 32000.000 kHz
  // ClkPer:  32000.000 kHz
  // ClkCPU:  32000.000 kHz
  CCP=CCP_IOREG_gc;
  CLK.PSCTRL=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) | CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;

  // Internal 32 MHz RC osc. calibration reference clock source: 32.768 kHz Internal Osc.
  //OSC.DFLLCTRL&= ~(OSC_RC32MCREF_bm | OSC_RC2MCREF_bm);
  // Enable the autocalibration of the internal 32 MHz RC oscillator
  DFLLRC32M.CTRL|=DFLL_ENABLE_bm;

  // Wait for the internal 32 MHz RC oscillator to stabilize
  while ((OSC.STATUS & OSC_RC32MRDY_bm)==0);


  // Select the system clock source: 32 MHz Internal RC Osc.
  CCP=CCP_IOREG_gc;
  CLK.CTRL=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC32M_gc;

  // Disable the unused oscillators: 2 MHz, external clock/crystal oscillator, PLL
  OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

  // Peripheral Clock output: Disabled
  PORTCFG.CLKEVOUT=(PORTCFG.CLKEVOUT & (~PORTCFG_CLKOUT_gm)) | PORTCFG_CLKOUT_OFF_gc;
}

cmdState_t *CLIStateSerialUsb;
FILE usbStream;

#ifdef USENET
cmdState_t *CLIStateSerialUdp;
FILE udpStream;
#endif

streamBuffers_t udpBuffers;

//#include <avr/iox128a1.h>

// Define CPU clock frequency (if not already defined)
#ifndef F_CPU
   // Enable/Disable internal oscillators
   //#define F_CPU 2000000
   #define F_CPU 32000000
   // Enable/Disable external 14.7456 MHz oscillator
   //#define F_CPU 14745600
#endif


portSHORT main( void )
{
  hardwareInit();

  xSerialPortInitMinimal(); //Przerobić dodać drigi Rs485
  CLIStateSerialUsb  = xmalloc(sizeof(cmdState_t));


  loadConfiguration();

  initQueueStreamUSB(&usbStream);
  VtyInit(CLIStateSerialUsb, &usbStream);


  sei();

  xTaskCreate(vTaskVTYusb,    NULL /*"VTY"            */, STACK_SIZE_VTY,         (void *)(CLIStateSerialUsb),            1, &xHandleVTY_USB);

  vTaskStartScheduler();

  return 0;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  for( ;; )
  {
    vCoRoutineSchedule();
  }
}

void vApplicationTickHook( void )
{
  static uint16_t tickCntr = configTICK_RATE_HZ;
  if (--tickCntr == 0)
  {
    tickCntr = configTICK_RATE_HZ;
#ifdef USENET
    arpTimer();
#endif
  }
}
