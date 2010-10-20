#include "Rs485_prot.h"

#include <util/crc16.h>

static void    sendPing(uint8_t addr, uint8_t pingLen);
static uint8_t receivePong(uint8_t addr, uint8_t pingLen);
static void    sendHello(uint8_t addr);
static uint8_t receiveHello(uint8_t* response, uint8_t maxSize);

// ********************* Those function have to be implemented in your project *************
void takeRs485(void)                              { }
void releaseRs485(void)                           { }
void uartRs485SendByte(uint8_t c)                 { c=0; }
uint8_t rs485Receive(uint8_t *c, uint8_t timeout) {  timeout = 0; c = NULL; return 1; }
uint8_t flushRs485RecBuffer(void)                 {return 1;}

// ********************* Hiden Functions ***************************************************
void sendPing(uint8_t addr, uint8_t pingLen)
{
  uint16_t crc = _crc_xmodem_update(0, SYNC);
  uartRs485SendByte(SYNC);

  crc = _crc_xmodem_update(crc, addr);
  uartRs485SendByte(addr);

  crc = _crc_xmodem_update(crc, rPING);
  uartRs485SendByte(rPING);

  crc = _crc_xmodem_update(crc, pingLen);
  uartRs485SendByte(pingLen);
  
  for (uint8_t i=0; i < pingLen; i++)
  {
    crc = _crc_xmodem_update(crc, i);
    uartRs485SendByte(i);
  }
  uartRs485SendByte((uint8_t)(crc >> 8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));
}

uint8_t receivePong(uint8_t addr, uint8_t dataLen)
{ 
  addr = 0;
  uint8_t data;
  uint16_t crc;
   
  //SYNC
  if(rs485Receive(&data, 20) == pdFALSE)
    return 1;
  
  if (data != SYNC)
    return 2;
  crc = _crc_xmodem_update(0, data);

  //Adres ma być wartość 0
  if(rs485Receive(&data, 1) == 0)
    return 3;

  if (data != 0)
    return 4;
  
  crc = _crc_xmodem_update(crc, data);

  //Kod rozkazu. Ma być rPING
  if(rs485Receive(&data, 1) == pdFALSE)
    return 5;
  if (data != rPING)
    return 6;
  crc = _crc_xmodem_update(crc, data);

  //Długość odpowiedzi
  if(rs485Receive(&data, 1) == pdFALSE)
    return 7;
  if (data != dataLen)
    return 8;
  crc = _crc_xmodem_update(crc, data);

  
  uint8_t temp;
  for (temp=0; temp < dataLen; temp++)
  {
    if(rs485Receive(&data, 5) == pdFALSE)
      return 2*temp+9;
    crc = _crc_xmodem_update(crc, data);
    if (data != temp)
      return 2*temp+10; 
  }

  uint8_t crcHi;
  uint8_t crcLo;
  if(rs485Receive(&crcHi, 1) != pdTRUE)
    return 2*temp+11;
  
  if(xQueueReceive(xRs485Rec, &crcLo, 1) != pdTRUE)
    return 2*temp+12;

  if (crcHi != (uint8_t)(crc>>8))
    return 254;
  if (crcLo != (uint8_t)(crc & 0xFF))
    return 255;
  
  return 0; 
}

static void    sendHello(uint8_t addr)
{
  uint16_t crc = _crc_xmodem_update(0, SYNC);
  uartRs485SendByte(SYNC);

  crc = _crc_xmodem_update(crc, addr);
  uartRs485SendByte(addr);

  crc = _crc_xmodem_update(crc, rPING);
  uartRs485SendByte(rHELLO);

  crc = _crc_xmodem_update(crc, 0);
  uartRs485SendByte(0);
  
  uartRs485SendByte((uint8_t)(crc >> 8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));
}

static uint8_t receiveHello(uint8_t* response, uint8_t maxSize)
{
  uint16_t crc;
  uint8_t data; 
  //SYNC
  if(rs485Receive(&data, 20) == pdFALSE)
    return 8;
  
  if (data != SYNC)
    return 2;
  crc = _crc_xmodem_update(0, data);

  //Adres ma być wartość 0
  if(rs485Receive(&data, 1) == 0)
    return 3;

  if (data != 0)
    return 4;
  
  crc = _crc_xmodem_update(crc, data);

  //Kod rozkazu. Ma być rPING
  if(rs485Receive(&data, 1) == pdFALSE)
    return 5;
  if (data != rHELLO)
    return 6;
  crc = _crc_xmodem_update(crc, data);

  //Długość odpowiedzi
  if(rs485Receive(&data, 1) == pdFALSE)
    return 7;
  if (data != maxSize)
    return 1;
  crc = _crc_xmodem_update(crc, data);

  
  for (data=0; data < maxSize; data++)
  {
    if(rs485Receive(response, 5) == pdFALSE)
      return 20;
    crc = _crc_xmodem_update(crc, *((uint8_t *)(response)));
    response++; 
  }

  uint8_t crcHi;
  uint8_t crcLo;
  if(rs485Receive(&crcHi, 1) != pdTRUE)
    return 21;
  
  if(xQueueReceive(xRs485Rec, &crcLo, 1) != pdTRUE)
    return 22;

  if (crcHi != (uint8_t)(crc>>8))
    return 254;
  if (crcLo != (uint8_t)(crc & 0xFF))
    return 255;
  
  return 0; 
}

// ************************ Rs485 API ************************************

uint8_t rs485ping(uint8_t devAddr)
{
  takeRs485();
  const int len = 8;    
  sendPing(devAddr, len);
  uint8_t result = receivePong(devAddr, len);
  flushRs485RecBuffer();

  releaseRs485();
  return result;
}

uint8_t rs485hello(uint8_t devAddr, void *data, uint8_t maxSize)
{
  takeRs485();
  sendHello(devAddr);
  uint8_t result = receiveHello((uint8_t *)(data), maxSize); 
  releaseRs485();
  return result;
}


uint8_t rs485xModemFlash(struct ramPlikFd *file, uint8_t devAddr)
{
  uint16_t crc;
  uint8_t  blad = 0;
  uint8_t  data;
  uint8_t  nrBloku;
  uint8_t  lRetransmisji;
  
  takeRs485();                                  // Zajmowanie magistrali Rs485

  // Wysyłanie polecenia restartu
  crc = _crc_xmodem_update(0, SYNC);            uartRs485SendByte(SYNC);
  crc = _crc_xmodem_update(crc, devAddr);       uartRs485SendByte(devAddr);
  crc = _crc_xmodem_update(crc, rFLASH);        uartRs485SendByte(rFLASH);
  crc = _crc_xmodem_update(crc, 1);             uartRs485SendByte(1);
  crc = _crc_xmodem_update(crc, devAddr);       uartRs485SendByte(devAddr);
  uartRs485SendByte((uint8_t)(crc>>8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));
 

  // Odbieranie odpowiedzi po wysłaniu polecenia restartu
  //Odbieranie SYNC lub C
  if(rs485Receive(&data, 100) != pdTRUE)
    blad = 1;                                  // Timeout
  crc = _crc_xmodem_update(0, data);
 
  if ((blad == 0) && (data == 'C'))
  {
    blad = 253;                                //Na urządzeniu jest wgrany tylko bootloader
  }
  else
  {
    if (data != SYNC)
      blad = 2;
  }
 
  //Odbieranie odpowiedzi programu. Program zresetuje się by uruchomić bootloadera.
  //Odbieranie adresu
  if (blad == 0)  //Odbieranie adresu (powinno być 0)
  {
    if(rs485Receive(&data, 1) != pdTRUE)
      blad = 3;
    else
    {
      if (data != 0)
        blad = 4;
    }
    crc = _crc_xmodem_update(crc, data);
  }
   
  //Odbieranie kodu rozkazu
  if (blad == 0)
  {
    if(rs485Receive(&data, 1) != pdTRUE)
      blad = 5;
    else
    {
      if (data != rFLASH)
        blad = 6;
    }
    crc = _crc_xmodem_update(crc, data);
  }
 
  //Odbieranie długości danych w rozkazie
  if (blad == 0)
  {
    if(rs485Receive(&data, 1) != pdTRUE)
      blad = 7;
    else
    {
      if (data != 1)
        blad = 8;
    }
    crc = _crc_xmodem_update(crc, data);
  }
 
  //Odbieranie danych w rozkazie
  if (blad == 0)
  {
    if(rs485Receive(&data, 1) != pdTRUE)
      blad = 9;
    else
    {
      if (data != devAddr)
        blad = 10;
    }
    crc = _crc_xmodem_update(crc, data);
  }
   
  //Odbieranie CRC Hi
  if (blad == 0)
  {
    if (rs485Receive(&nrBloku, 1) != pdTRUE)
      blad = 11;
    else
    {
      if ((uint8_t)(crc>>8) != nrBloku)
        blad = 12;
    }
  }
   
  //Odbieranie CRC Lo
  if (blad == 0)
  {
    if (rs485Receive(&nrBloku, 1) != pdTRUE)
      blad = 13;
    else
    {
      if ((uint8_t)(crc & 0xFF) != nrBloku)
        blad = 14;
    }    
  }
   
  if ((blad != 0) && (blad != 253))
  {
    releaseRs485();
    flushRs485RecBuffer();  
    return 1;
  }

// Wysyłanie polecenia do bootloadera
  if (blad == 0)
  {
    vTaskDelay(100);
     
    crc = _crc_xmodem_update(0, SYNC);            uartRs485SendByte(SYNC);
    crc = _crc_xmodem_update(crc, devAddr);       uartRs485SendByte(devAddr);
    crc = _crc_xmodem_update(crc, rFLASH);        uartRs485SendByte(rFLASH);
    crc = _crc_xmodem_update(crc, 1);             uartRs485SendByte(1);
    crc = _crc_xmodem_update(crc, devAddr);       uartRs485SendByte(devAddr);
    uartRs485SendByte((uint8_t)(crc>>8));
    uartRs485SendByte((uint8_t)(crc & 0xFF));
 
    //Odbieranie odpowiedzi od Bootloadera
    if(rs485Receive(&data, 150) != pdTRUE)
      blad = 15;

    if (data != 'C')
      blad = 16;
    else
      blad = 0;
  }
   
  if (blad != 0)
  {
    releaseRs485();
    flushRs485RecBuffer();  
    return 1;
  }
 
  uint8_t liczbaBlokow = file->wpis->rozmiarHi * 2;
  if (file->wpis->rozmiarLo == 128)
    liczbaBlokow++;
   
  nrBloku = 1;
  lRetransmisji = 0;
   
  while (nrBloku <= liczbaBlokow)
  {
    crc = 0;
    uartRs485SendByte(SOH);
    uartRs485SendByte(nrBloku);
    data = (uint8_t)(~nrBloku);
    uartRs485SendByte(data);
 
    for (blad = 0; blad < 128; blad++)           //wysyłanie danych. Zmianna Blad jest tymczasowa
    {
      if (ramDyskCzytajBajtZPliku(file, &data) != 0)
        data = 0;
      crc = _crc_xmodem_update(crc, data);
      uartRs485SendByte(data);
    }
    uartRs485SendByte((uint8_t)(crc>>8));
    uartRs485SendByte((uint8_t)(crc & 0xFF));
 
    if(rs485Receive(&data, 100) != pdTRUE)
    {
      blad = 250;
    }
     
    flushRs485RecBuffer();

    if (data == ACK)
    {
      nrBloku ++;
      lRetransmisji = 0;
      continue;
    }
 
    if (data == CAN)
      blad = 249;
 
    lRetransmisji ++;
     
    if (lRetransmisji == 3)
    {
      blad = 249;
      break;
    }
  }
 
  if (blad == 0)
  {
    uartRs485SendByte(EOT); 
    if(rs485Receive(&data, 25) == pdTRUE)
    {
      if (data == ACK)
      {
        uartRs485SendByte(EOT);
        rs485Receive(&data, 25);
      }
    }
  }
  
  flushRs485RecBuffer();  
  releaseRs485();
  return 0;
}

uint8_t rs485curtainUp(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos)
{
  uint16_t crc = 0;
  
  crc = _crc_xmodem_update(crc, SYNC);
  uartRs485SendByte(SYNC);

  crc = _crc_xmodem_update(crc, deviceAddr);
  uartRs485SendByte(deviceAddr);
  if (curtainNo == 0)
  {
    crc = _crc_xmodem_update(crc, rPodniesRolete1);
    uartRs485SendByte(rPodniesRolete1);
  }
  else
  {
    crc = _crc_xmodem_update(crc, rPodniesRolete2);
    uartRs485SendByte(rPodniesRolete2); 
  }
  
  crc = _crc_xmodem_update(crc, 1);    uartRs485SendByte(1);
  crc = _crc_xmodem_update(crc, pos);  uartRs485SendByte(pos);
  
  uartRs485SendByte((uint8_t)(crc>>8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));
  
  return 0;
}

uint8_t rs485curtainDown(uint8_t deviceAddr, uint8_t curtainNo, uint8_t pos)
{
  uint16_t crc = 0;
  
  crc = _crc_xmodem_update(crc, SYNC);            uartRs485SendByte(SYNC);
  crc = _crc_xmodem_update(crc, deviceAddr);    uartRs485SendByte(deviceAddr);
  if (curtainNo == 0)
  {
    crc = _crc_xmodem_update(crc, rOpuscRolete1); uartRs485SendByte(rOpuscRolete1);
  }
  else
  {
    crc = _crc_xmodem_update(crc, rOpuscRolete2); uartRs485SendByte(rOpuscRolete2); 
  }

  crc = _crc_xmodem_update(crc, 1);               uartRs485SendByte(1);
  crc = _crc_xmodem_update(crc, pos);         uartRs485SendByte(pos);
  
  uartRs485SendByte((uint8_t)(crc>>8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));

  return 0;
}
