#include "spi_task.h"

uint8_t * const spiTxbuffer0 = (uint8_t *) SPI_TX_BUFFER;
uint8_t * const spiTxbuffer1 = (uint8_t *) (SPI_TX_BUFFER + 512);
uint8_t * const spiRxbuffer0 = (uint8_t *) SPI_RX_BUFFER;
uint8_t * const spiRxbuffer1 = (uint8_t *) (SPI_RX_BUFFER + 512);


void XpNetReceiverTask ( void *pvParameters )
{
  FILE *debugStr = (FILE *) pvParameters;

  enum REC_STATE recState = ST_SYNC;
  uint8_t bufNo = 0;
  uint8_t dtaLen = 0;
  uint8_t crcLo;
  uint8_t crcHi;


  for( ; ; )
  {
    uint8_t dta;
    uint16_t crc;
    if (!xQueueReceive(udpSocket->Rx, &dta, 100))
      continue;

    switch (recState)
    {
    case ST_SYNC:
      if (dta == 0xA5)
        recState = ST_CMD;
      crc = 0;
      break;

    case ST_CMD:
      crc = _crc16_update(crc, dta);
      recState = ST_LEN;
      bufNo = dta;
      break;

    case ST_LEN:
      crc = _crc16_update(crc, dta);
      recState = ST_DATA;
      dtaLen=dta;
      break;

    case ST_DATA:
      crc = _crc16_update(crc, dta);
      if (bufNo < NO_OF_SPI2SERIAL_RX_BUF)
      {
        xQueueSend(xSpi2SerialTx[bufNo], &dta, 0);
      } //dopisać obsługę resetowania
      else
      {
        if (debugStr != NULL)
          fprintf_P(debugStr, PSTR("Reset is not implemented yet"));
      }
      dtaLen--;
      if (dtaLen == 0)
      {
        recState = ST_CRC_HI;
      }
      break;
    case ST_CRC_HI:
      crcHi = dta;
      recState = ST_CRC_LO;
      break;

    case ST_CRC_LO:
      crcLo = dta;
      recState = ST_SYNC;

      if ((crcLo + crcHi<<8) != crc)
        if (debugStr != NULL)
          fprintf_P(debugStr, PSTR("CRC error"));
      break;
    }
  }
}

void XpNetTransmitterTask ( void *pvParameters )
{
  FILE *debugStr = (FILE *) pvParameters;
  uint8_t bufIdx;

  for(bufIdx=0; bufIdx<NO_OF_SPI2SERIAL_RX_BUF; bufIdx++)
  {
    if (bufIdx == NO_OF_SPI2SERIAL_RX_BUF)
        bufIdx = 0;

    uint16_t crc = 0;
    uint8_t dta = 0xA5;;
    uint8_t dtaLen = uxQueueMessagesWaiting(xSpi2SerialRx[bufIdx]);
    if (dtaLen == 0)
      continue;

    crc = _crc16_update(crc, dtaLen);
    xQueueSend(udpSocket->Tx, &dta, 0);    //SYNC - 1bajt ma zawsze wartość 0xA5
    crc = _crc16_update(crc, dta);
    xQueueSend(udpSocket->Tx, &bufIdx, 0);  //TYPE - 1 bajt, który  określa numer portu lub polecenie. Polecenia: 254 wyłącz zasilanie, 255 włącz zasilanie
    crc = _crc16_update(crc, dta);
    xQueueSend(udpSocket->Tx, &dtaLen, 0);  //LENGTH - 1 bajt  który określa długość wiadomości (wiadomość nie może przekrczać 200 bajtów bo mam mało pamięci w CPU)
    crc = _crc16_update(crc, dta);
    while (dtaLen >0)       //WIADOMOSC - no dane, które odebrano lub mają być wyzłane pod określony port szeregowy
    {
        xQueueReceive(xSpi2SerialRx[bufIdx], &dta, 0);
        xQueueSend(udpSocket->Tx, &dta, 0);
        crc = _crc16_update(crc, dta);
        dtaLen--;
    }
    dta = crc>>8;
    xQueueSend(udpSocket->Tx, &dta, 0);

    dta = crc;
    xQueueSend(udpSocket->Tx, &dta, 0);
  }
}

void spiTask ( void *pvParameters )
{
  FILE *netstackDebug = (FILE *) pvParameters;
  uint16_t plen;

  nicInit();
  ipInit();
  arpInit();
  icmpInit();


  //TODO    init_ip_arp_udp_tcp (mymac, ipGetConfig()->ip, MYWWWPORT);

  uint8_t spiDevIdx = 0;
  for ( ; ; )
  {
    vTaskDelay ( 0 );         //Zastąpić oczekiwaniem na zwolnienie semafora. Semafor zostaje zwolniony po odebraniu przerwania od ENC

    // get the next new packet:
    plen = nicPoll();
    /*plen will ne unequal to zero if there is a valid
    * packet (without crc error) */
    if ( plen==0 )
    {


      handleSpiDev(spiDevIdx);

      flushSpi2SerialRxBuffers();

      flushUdpQueues();
      flushTcpQueues();
      //flush HTTP long file queue


      spiDevIdx++;
      spiDevIdx &= 0x07;
      continue;
    }

    if(nicState.layer2.ethHeader->type == htons(ETHTYPE_IP))             // process an IP packet
    {
      arpIpIn();
      netstackIPv4Process();
    }
    else if(nicState.layer2.ethHeader->type == htons(ETHTYPE_ARP))       // process an ARP packet
    {
      arpArpIn();
    }
    else
    {
      if (netstackDebug != NULL)
      {
        fprintf_P(netstackDebug, PSTR("Unknown packet\r\n"));
      }
    }
  }
}

void handleSpiDev(uint8_t spiDevNo)
{
  uint8_t *bufTxPtr;

  uint8_t bufTx0_len=0;
  uint8_t bufTx1_len=0;

  uint8_t bufRx0_len=0;
  uint8_t bufRx1_len=0;

  uint8_t tmpDta;
  uint8_t tmpDtaLo;
  uint8_t tmpDtaHi;

  uint8_t qeueNo;

  uint8_t i;
  uint8_t isDataOnSlave = 1;

  //Przygotownanie bufora nadawczego do SPI.
  qeueNo = 2*spiDevNo;
  bufTxPtr = spiTxbuffer0;
  while (xQueueReceive(xSpi2SerialTx[qeueNo], &tmpDta, 0))
  {
    tmpDtaLo = (tmpDta & 0x0F);
    tmpDtaHi = ((tmpDta>>4) & 0x0F) | 0x10;

    *(bufTxPtr++) = tmpDtaLo;
    *(bufTxPtr++) = tmpDtaHi;
    bufTx0_len++;
  }
  qeueNo++;
  bufTxPtr = spiTxbuffer1;
  while (xQueueReceive(xSpi2SerialTx[qeueNo], &tmpDta, 0))
  {

    tmpDtaLo = (tmpDta & 0x0F) | 0x20;
    tmpDtaHi = ((tmpDta>>4) & 0x0F) | 0x30;

    *(bufTxPtr++) = tmpDtaLo;
    *(bufTxPtr++) = tmpDtaHi;
    bufTx1_len++;
  }
  spiEnableDev(spiDevNo);



  //Wysłanie zawartości bufora TxSpi oraz odczyt od modułów slave
#define SPI_TRANSMISSION                     \
  {                                          \
    tmpDta = spiSend(spiTxbuffer0[i]);       \
    if (tmpDta == 0xCF)                      \
    {                                        \
        isDataOnSlave = 0;                   \
        continue;                            \
    }                                        \
    if ((tmpDta & 0x20) == 0)                \
      spiRxbuffer0[bufRx0_len++] = tmpDta;   \
    else                                     \
      spiRxbuffer1[bufRx1_len++] = tmpDta;   \
                                             \
    if (bufRx0_len==254) break;              \
    if (bufRx1_len==254) break;              \
  }

  for (i=0; i<bufTx0_len; i++)
    SPI_TRANSMISSION

  for (i=0; i<bufTx1_len; i++)
    SPI_TRANSMISSION

  while(isDataOnSlave)
    SPI_TRANSMISSION
#undef SPI_TRANSMISSION
  spiDisableDev(spiDevNo);
///Przetwarzanie danych odebranych z SPI i umieszczanie ich w odpowiednim buforze

  qeueNo = 2*spiDevNo;
  for (i=0; i<bufRx0_len; i+=2)
  {
      uint8_t tmpValue = ((spiRxbuffer0[i] & 0x0F) | ((spiRxbuffer0[i+1]<<4) & 0xF0));
      xQueueSend(xSpi2SerialRx[qeueNo], &tmpValue, 0);
  }
  qeueNo++;
  for (i=0; i<bufRx1_len; i+=2)
  {
      uint8_t tmpValue = ((spiRxbuffer1[i] & 0x0F) | ((spiRxbuffer1[i+1]<<4) & 0xF0));
      xQueueSend(xSpi2SerialRx[qeueNo], &tmpValue, 0);
  }
}


void flushSpi2SerialRxBuffers()
{
    uint8_t bufIdx;
    for(bufIdx=0; bufIdx<NO_OF_SPI2SERIAL_RX_BUF; bufIdx++)
    {
        uint8_t dta = 0xA5;;
        uint8_t dtaLen = uxQueueMessagesWaiting(xSpi2SerialRx[bufIdx]);
        if (dtaLen == 0)
            continue;


        xQueueSend(udpSocket->Tx, &dta, 0);    //SYNC - 1bajt ma zawsze wartość 0xA5
        xQueueSend(udpSocket->Tx, &bufIdx, 0);  //TYPE - 1 bajt, który  określa numer portu lub polecenie. Polecenia: 254 wyłącz zasilanie, 255 włącz zasilanie
        xQueueSend(udpSocket->Tx, &dtaLen, 0);  //LENGTH - 1 bajt  który określa długość wiadomości (wiadomość nie może przekrczać 200 bajtów bo mam mało pamięci w CPU)
        while (dtaLen >0)       //WIADOMOSC - no dane, które odebrano lub mają być wyzłane pod określony port szeregowy
        {
            xQueueReceive(xSpi2SerialRx[bufIdx], &dta, 0);
            xQueueSend(udpSocket->Tx, &dta, 0);
            dtaLen--;
        }
        flushUdpQueues();
        //Zastanawiam się nad dodaniem na końcu jeszcze sumy kontrolnej. CRC - 2 bajty z sumą kontrolną. Teraz można to ignorować lub wysyłać wartość 0x0000
    }
}

void initQeuesSpi2Serial()
{
  portENTER_CRITICAL();
  {
      uint8_t i;
      for (i=0; i<NO_OF_SPI2SERIAL_TX_BUF; i++)
      {
        xSpi2SerialTx[i]  =   xQueueCreateExternal(255, 1, (void *)(SPI2SERIAl_TX_BUFFERS + i*OF_SPI2SERIAL_TX_BUF_SIZE));
      }
      for (i=0; i<NO_OF_SPI2SERIAL_RX_BUF; i++)
      {
        xSpi2SerialRx[i]  =   xQueueCreateExternal(255, 1, (void *)(SPI2SERIAl_RX_BUFFERS + i*OF_SPI2SERIAL_RX_BUF_SIZE));
      }
  }
  portEXIT_CRITICAL();
}
