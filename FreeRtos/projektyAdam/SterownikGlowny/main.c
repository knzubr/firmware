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
#include "task.h"
//#include "enc_task.h"
#include "protRs485.h"
#include "serial.h"

struct sterRolet sterownikiRolet[MAKS_L_STER_ROLET];

xQueueHandle xVtyRec;
xQueueHandle xRs485Rec;

xQueueHandle xVtyTx;
xQueueHandle xRs485Tx;

//xSemaphoreHandle xSemaphoreVty;
//xSemaphoreHandle xSemaphoreRs485;
//xSemaphoreHandle xSemaphoreVtyBusy;
//xSemaphoreHandle xSemaphoreRs485Busy;

#ifdef ENABLE_RESET_COUNTER
static void prvIncrementResetCount( void );
#endif

void vApplicationIdleHook( void );

/**
 * Proces odpowiedzialny za obsługę VTY
 * @param pvParameters ignorowane parametry
 */
void vTaskVTY( void * pvParameters );

/**
 * Proces odpowiedzialny za obsługę komunikacji po magistrali Rs485
 * @param pvParameters ignorowane parametry
 */
void vTaskMag(void *pvParameters);

/*-----------------------------------------------------------*/

void vTaskVTY( void * pvParameters )
{
  cmdState_t *state = (cmdState_t *)(pvParameters);
  cmdlineInputFunc('\r', state);
  
  static char znak;
  for( ;; )
  {
    if( xQueueReceive( xVtyRec, &znak, portMAX_DELAY))
    {
      cmdlineInputFunc(znak, state);
      cmdlineMainLoop(state);
    }  
  }
}

void vTaskMag(void *pvParameters)
{
  pvParameters = NULL;

  static uint8_t adr;
  static char znak;
  for(;;)
  {
    if(xQueueReceive(xRs485Rec, &znak, portMAX_DELAY))
    {
      sendPing(adr, 8, uartRs485SendByte);
//      if (sprOdebrPing(adr, 8, xRs485Rec))
//      {
//        rprintf("Znaleziono ster adres %x", adr);
//      }
      vTaskDelay(10);
    }
    adr++;
  }
}

xTaskHandle xHandleVTY;
xTaskHandle xHandleRs485;
xTaskHandle xHandleEnc;

portSHORT main( void )
{
  MCUCR |= _BV(SRE);          //Włączenie pamięci zewnętrznej
  ramDyskInit();              //Inicjalizacja Ram dysku
#ifdef ENABLE_RESET_COUNTER
  prvIncrementResetCount();
#endif

  hardwareInit();
//  spiInit(disableAllSpiDevices);
//  spiInit();
  xSerialPortInitMinimal();
  
  VtyInit();
  cmdState_t *CLIState = cmdStateCreate(64, VtyPutChar);

  xTaskCreate(vTaskVTY, NULL /*"VTY"*/, STACK_SIZE_VTY, (void *)(CLIState), 1, &xHandleVTY);
//  xTaskCreate(encTask,  NULL /*"ENC"*/, STACK_SIZE_ENC, (void *)(CLIState), 1, &xHandleEnc);
  
//  xTaskCreate(vTaskMag, "Rs485", STACK_SIZE_VTY, NULL, tskIDLE_PRIORITY, &xHandleRs485);
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

#ifdef ENABLE_RESET_COUNTER
static void prvIncrementResetCount( void )
{
  unsigned portCHAR ucCount;
  eeprom_read_block( &ucCount, mainRESET_COUNT_ADDRESS, sizeof( ucCount ) );
  ucCount++;
  eeprom_write_byte( mainRESET_COUNT_ADDRESS, ucCount );
}
#endif
