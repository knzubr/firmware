#include "protRs485.h"

#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "protocol1.h"

//#define DEBUG_RS485

void sendPing(uint8_t addr, uint8_t pingLen, void (*sendFunc)(uint8_t))
{
  uint16_t crc = _crc_xmodem_update(0, SYNC);
  sendFunc(SYNC);

  crc = _crc_xmodem_update(crc, addr);
  sendFunc(addr);

  crc = _crc_xmodem_update(crc, rPING);
  sendFunc(rPING);

  crc = _crc_xmodem_update(crc, pingLen);
  sendFunc(pingLen);
  
  for (uint8_t i=0; i < pingLen; i++)
  {
    crc = _crc_xmodem_update(crc, i);
    sendFunc(i);
  }
  sendFunc((uint8_t)(crc >> 8));
  sendFunc((uint8_t)(crc & 0xFF));
}


uint8_t sprOdebrPing(uint8_t addr, uint8_t dataLen, cmdState_t *state)
{
#ifndef DEBUG_RS485
  state = NULL;
#endif
  
  addr = 0;
  uint8_t data;
  uint16_t crc;
   
  //SYNC
  if(xQueueReceive(xRs485Rec, &data, 20) == pdFALSE)
    return 1;
#ifdef DEBUG_RS485
  fprintf(&state->myStdInOut, "0x%x ", data);
#endif
  if (data != SYNC)
    return 2;
  crc = _crc_xmodem_update(0, data);

  //Adres ma być wartość 0
  if(xQueueReceive(xRs485Rec, &data, 1) == pdFALSE)
    return 3;
#ifdef DEBUG_RS485
  fprintf(&state->myStdInOut, "0x%x ", data);
#endif
  if (data != 0)
    return 4;
  crc = _crc_xmodem_update(crc, data);

  //Kod rozkazu. Ma być rPING
  if(xQueueReceive(xRs485Rec, &data, 1) == pdFALSE)
    return 5;
#ifdef DEBUG_RS485
  fprintf(&state->myStdInOut, "0x%x ", data);
#endif
  if (data != rPING)
    return 6;
  crc = _crc_xmodem_update(crc, data);

  //Długość odpowiedzi
  if(xQueueReceive(xRs485Rec, &data, 1) == pdFALSE)
    return 7;
#ifdef DEBUG_RS485
  fprintf(&state->myStdInOut, "0x%x ", data);
#endif
  if (data != dataLen)
    return 8;
  crc = _crc_xmodem_update(crc, data);

  
  uint8_t temp;
  for (temp=0; temp < dataLen; temp++)
  {
    if(xQueueReceive(xRs485Rec, &data, 5) == pdFALSE)
      return 2*temp+9;
    crc = _crc_xmodem_update(crc, data);
#ifdef DEBUG_RS485
  fprintf(&state->myStdInOut, "0X%x ", data);
#endif
    if (data != temp)
      return 2*temp+10; 
  }

  uint8_t crcHi;
  uint8_t crcLo;
  if(xQueueReceive(xRs485Rec, &crcHi, 1) != pdTRUE)
    return 2*temp+11;
  
  if(xQueueReceive(xRs485Rec, &crcLo, 1) != pdTRUE)
    return 2*temp+12;

  if (crcHi != (uint8_t)(crc>>8))
    return 254;
  if (crcLo != (uint8_t)(crc & 0xFF))
    return 255;
  
  return 0; 
}

uint8_t czyscBufOdb485(cmdState_t *state)
{
  uint8_t temp;
  uint8_t wynik = 0;
  if (state != NULL)
  {
    fprintf(&state->myStdInOut, "Czyszczenie bufora odbiorczego Rs485: ");  
    while(xQueueReceive(xRs485Rec, &temp, 10) == pdTRUE)
    {
      fprintf(&state->myStdInOut, "0X%x ", temp);
      wynik++;
    }
    fprintf(&state->myStdInOut, "OK\r\n");  
  }
  else
    while(xQueueReceive(xRs485Rec, &temp, 10) == pdTRUE)
      wynik++;
    
  return wynik;
}
