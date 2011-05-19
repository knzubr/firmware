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

/**
 * Coroutine, that is responsible for reading keys
 * @param pvParameters ignorowane parametry
 */
static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

/**
 * Coroutine function, that is responsible for Diode
 * @param pvParameters ignorowane parametry
 */
static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

void vApplicationIdleHook( void );


/*-----------------------------------------------------------*/

/* Device address on RS 485 bus */
uint8_t address;


portSHORT main( void )
{
  hardwareInit();

  xCoRoutineCreate(vKlawisze, 0, 0);
  xCoRoutineCreate(vDioda, 0, 0);
  xCoRoutineCreate(vDioda, 0, 1);

  vTaskStartScheduler();
  return 0;
}
/*-----------------------------------------------------------*/

static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  (void) uxIndex;           // get rid compiller worning. Theare is only one such coroutine. We don't need info about its index.

  crSTART( xHandle );
  for( ;; )
  {
    crDELAY( xHandle, 1);
  }
  crEND();
}

static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  crSTART( xHandle );
  for (;;)
  {
    crDELAY(xHandle, 100);
  }
  crEND();
}

void vApplicationIdleHook( void )
{
  for( ;; )
  {
    vCoRoutineSchedule();
  }
}
