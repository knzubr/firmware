#include "main.h"
#include "vty.h"
#include "ramdysk.h"
#include "protRs485.h"
#include "protocol1.h"
#include "mpc23s17.h"
#include "mcp3008.h"
#include "ds1305.h"

static void helpFunction           (cmdState_t *state);
static void statusFunction         (cmdState_t *state);
static void opuscFunction          (cmdState_t *state);
static void podniesFunction        (cmdState_t *state);
static void pingFunction           (cmdState_t *state);
static void goXmodemOdbierzFunction(cmdState_t *state);
static void goXmodemWyslijFunction (cmdState_t *state);
static void dodajRamPlikFunction   (cmdState_t *state);
static void kasujRamPlikFunction   (cmdState_t *state);
static void flashowanieModWyk      (cmdState_t *state);
#ifdef debugRamDysk
static void testujRamPlikFunction  (cmdState_t *state);
#endif
static void wypiszPlikiFunction    (cmdState_t *state);
static void edytujRamPlikFunction  (cmdState_t *state);
static void czytajRamPlikFunction  (cmdState_t *state);

static void ustawPortExtAFunction  (cmdState_t *state);

static void pokazCzasFunction      (cmdState_t *state);
static void ustawCzasFunction      (cmdState_t *state);
static void czytajAC_Function      (cmdState_t *state);

#ifdef testZewPamiec
static void testPamZewFunction     (cmdState_t *state);
#endif

struct ramPlikFd    fdVty;

uint8_t cmdErrno = 0;
uint8_t err1;
uint8_t err2;



void VtyInit(void)
{
  cmdlineInit();
  
  cmdlineAddCommand("help",    helpFunction);
  cmdlineAddCommand("status",  statusFunction);
  cmdlineAddCommand("ping",    pingFunction);
  cmdlineAddCommand("xodb",    goXmodemOdbierzFunction);
  cmdlineAddCommand("xwysl",   goXmodemWyslijFunction);
  cmdlineAddCommand("xflash",  flashowanieModWyk);
#ifdef testZewPamiec
  cmdlineAddCommand("rtest",   testPamZewFunction);
#endif
  cmdlineAddCommand("urp",     dodajRamPlikFunction);
  cmdlineAddCommand("krp",     kasujRamPlikFunction);
#ifdef debugRamDysk
  cmdlineAddCommand("trp",     testujRamPlikFunction);  
#endif
  cmdlineAddCommand("dir",     wypiszPlikiFunction);
  cmdlineAddCommand("erp",     edytujRamPlikFunction);
  cmdlineAddCommand("crp",     czytajRamPlikFunction);
  
  
  cmdlineAddCommand("podnies", podniesFunction);
  cmdlineAddCommand("opusc",   opuscFunction);
  cmdlineAddCommand("upa",     ustawPortExtAFunction);
  cmdlineAddCommand("czas",    pokazCzasFunction);
  cmdlineAddCommand("uczas",   ustawCzasFunction);
  cmdlineAddCommand("ac",      czytajAC_Function);
}

#define odczyt(ZNAK, TIMEOUT) xQueueReceive(xVtyRec, (ZNAK), (TIMEOUT))

static void statusFunction(cmdState_t *state)
{
  fprintf(&state->myStdInOut, "Liczba watkow: %d\r\n", uxTaskGetNumberOfTasks());
  if (cmdErrno != 0)
    fprintf(&state->myStdInOut, "Ostatni blad: %d (%d, %d)\r\n", cmdErrno, err1, err2);
  uint8_t miejsce = ramDyskLiczbaWolnychKlastrow();
  fprintf(&state->myStdInOut, "Miejsce w ram dysku %d/%d\r\n", miejsce,  L_KLASTROW);
  
  fprintf(&state->myStdInOut, "Adres struktury cli   0x%x\r\n", state);
  fprintf(&state->myStdInOut, "Adres bufora cli      0x%x\r\n", state->CmdlineBuffer);
  fprintf(&state->myStdInOut, "Adres bufora enc28j60 0x%x\r\n", Enc28j60_global.buf);
  
  uint16_t i;
  uint8_t *tmp = Enc28j60_global.buf;
  uint16_t lBladow = 0; 
  
  uint16_t *ptr = (uint16_t *)(tmp);
  ptr--;
  uint16_t l2 = *ptr;
  ptr--;
  uint16_t l1 = *ptr;
  
//  for (i=0; i<500; i++)
//  {
//    *tmp = (uint8_t)(i);
//    tmp++;
//    vTaskDelay(1);
//  }
//  tmp = Enc28j60_global.buf;
//  for (i=0; i<550; i++)
//  {
//    if (*tmp =! (uint8_t)(i))
//      lBladow++;
//    tmp++;
//    vTaskDelay(1);
//  }
//  fprintf(&state->myStdInOut, "Sterta %d %d\r\n", l1, l2);
}

static void pokazCzasFunction(cmdState_t *state)
{
  readTimeDecoded(&czasRtc);
  uint8_t godzina = 10*czasRtc.hours.syst24.cDzies + czasRtc.hours.syst24.cJedn;  
  uint8_t minuta =  10*czasRtc.minutes.cDzies + czasRtc.minutes.cJedn;
  uint8_t sekunda = 10*czasRtc.seconds.cDzies + czasRtc.seconds.cJedn;
  
  
  fprintf(&state->myStdInOut, "Aktualny czas %d:%d:%d\r\n", godzina, minuta, sekunda);
}

static void ustawCzasFunction(cmdState_t *state)
{
  uint8_t godzina =  cmdlineGetArgInt(1, state);
  uint8_t minuta  =  cmdlineGetArgInt(2, state);
  uint8_t sekunda =  cmdlineGetArgInt(3, state);
  
  ds1305start();

  uint8_t cDzies = godzina/10;
  uint8_t cJedn = godzina - cDzies*10;
  czasRtc.hours.syst24.cDzies = cDzies;
  czasRtc.hours.syst24.cJedn  = cJedn;
  
  cDzies = minuta/10;
  cJedn = minuta - cDzies * 10;
  czasRtc.minutes.cDzies = cDzies;
  czasRtc.minutes.cJedn  = cJedn;
  
  cDzies = sekunda/10;
  cJedn  = sekunda - cDzies * 10;
  czasRtc.seconds.cDzies = cDzies;
  czasRtc.seconds.cJedn  = cJedn;
  
  setTimeDecoded(&czasRtc);
  
  
//  uint8_t tab[8]= {1, 2, 3, 4, 5, 6, 7, 8};
  
//  uint8_t result;
//  result = ds1305writeMem(20, 8, tab);
//  if (result != 0)
//    fprintf(&state->myStdInOut, "Nieudana proba zapisu: %d\r\n", result);
//  result = ds1305readMem (20, 8, tab);
//  if (result != 0)
//    fprintf(&state->myStdInOut, "Nieudana proba odczytu: %d\r\n", result);
//  fprintf(&state->myStdInOut, "tab[0] %d, tab[4] %d, tab[7] %d\r\n", tab[0], tab[4], tab[7]);  
}

static void czytajAC_Function(cmdState_t *state)
{
  uint8_t nrWejscia = cmdlineGetArgInt(1, state);
  uint16_t wynik = MCP3008_getSampleSingle(nrWejscia);
  fprintf(&state->myStdInOut, "Wartosc probki na wejsciu %d: %d\r\n", nrWejscia, wynik);  
}

static void helpFunction(cmdState_t *state)
{
  fprintf(&state->myStdInOut, "Dostepne opcje to:\r\n");
  fprintf(&state->myStdInOut, "\tstatus\r\n");
  fprintf(&state->myStdInOut, "\tpodnies\r\n");
  fprintf(&state->myStdInOut, "\topusc\r\n");
  fprintf(&state->myStdInOut, "\tping\r\n");
  fprintf(&state->myStdInOut, "\txodb\r\n");
  fprintf(&state->myStdInOut, "\txwysl\r\n");
  fprintf(&state->myStdInOut, "\txflash\r\n");
  fprintf(&state->myStdInOut, "\turp\r\n");
  fprintf(&state->myStdInOut, "\tkrp\r\n");
  fprintf(&state->myStdInOut, "\tdir\r\n");
  fprintf(&state->myStdInOut, "\terp\r\n");
  fprintf(&state->myStdInOut, "\tcrp\r\n");
#ifdef testZewPamiec
  fprintf(&state->myStdInOut, "\trtest\r\n");
#endif
#ifdef debugRamDysk
  fprintf(&state->myStdInOut, "\ttrp\r\n");
#endif
}

static void opuscFunction(cmdState_t *state)
{
  uint8_t nrRolety;
  uint8_t nrSterownika;
  uint8_t wartosc;
  
  nrSterownika = cmdlineGetArgInt(1, state);
  nrRolety = cmdlineGetArgInt(2, state);
  nrRolety &= 0x01;
  wartosc = cmdlineGetArgInt(3, state);

  fprintf(&state->myStdInOut,   "Opuszczanie rolety\r\n");
  fprintf(&state->myStdInOut,   "\tsterownik %d\r\n", nrSterownika); 
  fprintf(&state->myStdInOut,   "\troleta    %d\r\n", nrRolety+1);
  if ((wartosc > 0) && (wartosc <=100))
    fprintf(&state->myStdInOut, "\tpozycja   %d\r\n", wartosc);

  uint16_t crc = 0;
  
  crc = _crc_xmodem_update(crc, SYNC);
  uartRs485SendByte(SYNC);
  crc = _crc_xmodem_update(crc, nrSterownika);
  uartRs485SendByte(nrSterownika);
  if (nrRolety == 0)
  {
    crc = _crc_xmodem_update(crc, rOpuscRolete1);
    uartRs485SendByte(rOpuscRolete1);
  }
  else
  {
    crc = _crc_xmodem_update(crc, rOpuscRolete1);
    uartRs485SendByte(rOpuscRolete2); 
  }

  crc = _crc_xmodem_update(crc, 1);
  uartRs485SendByte(1);
  crc = _crc_xmodem_update(crc, wartosc);
  uartRs485SendByte(wartosc);
  
  uartRs485SendByte((uint8_t)(crc>>8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));
}

static void podniesFunction(cmdState_t *state)
{
  uint8_t nrRolety;
  uint8_t nrSterownika;
  uint8_t wartosc;
  
  nrSterownika = cmdlineGetArgInt(1, state);
  nrRolety = cmdlineGetArgInt(2, state);
  nrRolety &= 0x01;
  wartosc = cmdlineGetArgInt(3, state);

  fprintf(&state->myStdInOut,   "Podnoszenie rolety\r\n");
  fprintf(&state->myStdInOut,   "\tsterownik %d\r\n", nrSterownika); 
  fprintf(&state->myStdInOut,   "\troleta    %d\r\n", nrRolety+1);
  if ((wartosc > 0) && (wartosc <=100))
    fprintf(&state->myStdInOut, "\tpozycja   %d\r\n", wartosc);

  uint16_t crc = 0;
  
  crc = _crc_xmodem_update(crc, SYNC);
  uartRs485SendByte(SYNC);

  crc = _crc_xmodem_update(crc, nrSterownika);
  uartRs485SendByte(nrSterownika);
  if (nrRolety == 0)
  {
    crc = _crc_xmodem_update(crc, rPodniesRolete1);
    uartRs485SendByte(rPodniesRolete1);
  }
  else
  {
    crc = _crc_xmodem_update(crc, rPodniesRolete2);
    uartRs485SendByte(rPodniesRolete2); 
  }
  
  crc = _crc_xmodem_update(crc, 1);
  uartRs485SendByte(1);
  crc = _crc_xmodem_update(crc, wartosc);
  uartRs485SendByte(wartosc);
  
  uartRs485SendByte((uint8_t)(crc>>8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));
}

static void ustawPortExtAFunction(cmdState_t *state)
{
  uint8_t wyjscie = cmdlineGetArgInt(1, state);
  MPC23s17SetDirA(0x00, 0);
  MPC23s17SetPortA(wyjscie, 0);
}

static void pingFunction(cmdState_t *state)
{
  static uint8_t nrSterownika;
  static uint8_t wynik;
  static uint8_t l_znakow;

  nrSterownika = (uint8_t)(cmdlineGetArgInt(1, state));
  l_znakow = cmdlineGetArgInt(2, state);

  wynik = czyscBufOdb485(NULL);
  if (wynik != 0)
    fprintf(&state->myStdInOut, "!!! W buforze Rs485 pozostalo %d bajtow\r\n", wynik);
    
  sendPing(nrSterownika, l_znakow, uartRs485SendByte);
//  czyscBufOdb485(state);  
  wynik = sprOdebrPing(nrSterownika, l_znakow, state);
  if (wynik != 0)
  {
    czyscBufOdb485(state);
    if (wynik == 1)
      fprintf(&state->myStdInOut, "Urzedzenie nr %d nie odpowiada\r\n", nrSterownika);
    else
      fprintf(&state->myStdInOut, "Blad nr %d w odpowiedzi urzadzenia nr %d\r\n", wynik, nrSterownika);
  }
  else
    fprintf(&state->myStdInOut, "Znaleziono urzadzanie nr %d\r\n", nrSterownika);
}

static void flashowanieModWyk(cmdState_t *state)
{
  //zablokuj proces do cyklicznego komunikowania się z modułami wykonawczymi

  uint8_t  nrUrzadzenia;                              //! Adres urządzenia, w którym zostanie wgrane nowe oprogramowanie
  uint8_t  data;
  uint8_t  blad;
  uint8_t  nrBloku;
  uint8_t  lRetransmisji;
  uint16_t crc;

  char *nazwaPliku;
 
  blad = czyscBufOdb485(NULL);
  if (blad != 0)
    fprintf(&state->myStdInOut, "!!! W budorze Rs485 pozostalo %d bajtow\r\n", blad);
  blad = 0;
  
  nrUrzadzenia= cmdlineGetArgInt(1, state); 
  nazwaPliku  = cmdlineGetArgStr(2, state);

  /*
  //Sprawdzanie, czy można połączyć się z urządzeniem
  sendPing(nrUrzadzenia, 8, uartRs485SendByte);
  uint8_t wynik = sprOdebrPing(nrUrzadzenia, 8, state);
  if (wynik != 0)
  {
    fprintf(&state->myStdInOut, "Brak urzadzenia\r\n");    
    return;
  }
  */
  
  //Sprawdzanie, czy istnieje odpowiedni plik z firmware
  if (ramDyskOtworzPlik(nazwaPliku, &fdVty) != 0)
  {
    fprintf(&state->myStdInOut, "Nie mozna otworzyc pliku %s\r\n", nazwaPliku);
    return;
  }
  
  //Wysyłanie polecenia restartu
  crc = _crc_xmodem_update(0, SYNC);            uartRs485SendByte(SYNC);
  crc = _crc_xmodem_update(crc, nrUrzadzenia);  uartRs485SendByte(nrUrzadzenia);
  crc = _crc_xmodem_update(crc, rFLASH);        uartRs485SendByte(rFLASH);
  crc = _crc_xmodem_update(crc, 1);             uartRs485SendByte(1);
  crc = _crc_xmodem_update(crc, nrUrzadzenia);  uartRs485SendByte(nrUrzadzenia);
  uartRs485SendByte((uint8_t)(crc>>8));
  uartRs485SendByte((uint8_t)(crc & 0xFF));

//    fprintf(&state->myStdInOut, "Wywylywanie urzadzenia %d (0x%x, 0x%x)\r\n", nrUrzadzenia, SYNC, nrUrzadzenia, rFLASH, 1, );

  ///Odbieranie odpowiedzi po wysłaniu polecenia restartu

  //Odbieranie SYNC lub C
  if(xQueueReceive(xRs485Rec, &data, 100) != pdTRUE)
    blad = 1;
  crc = _crc_xmodem_update(0, data);

  if ((blad == 0) && (data == 'C'))
  {
    blad = 253;                                 //Na urządzeniu jest wgrany tylko bootloader
    fprintf(&state->myStdInOut, "Wykonuje się kod bootloadera, który oczekuje startu transmisji\r\n");
  }

  if ((blad == 0) && (data != SYNC))
  {
    blad = 2;
  }

  //Odbieranie odpowiedzi programu. Program zresetuje się by uruchomić bootloadera.
  //Odbieranie adresu
  if (blad == 0)  //Odbieranie adresu (powinno być 0)
  {
    if(xQueueReceive(xRs485Rec, &data, 1) != pdTRUE)
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
    if(xQueueReceive(xRs485Rec, &data, 1) != pdTRUE)
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
    if(xQueueReceive(xRs485Rec, &data, 1) != pdTRUE)
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
    if(xQueueReceive(xRs485Rec, &data, 1) != pdTRUE)
      blad = 9;
    else
    {
      if (data != nrUrzadzenia)
        blad = 10;
    }
    crc = _crc_xmodem_update(crc, data);
  }
  
  //Odbieranie CRC Hi
  if (blad == 0)
  {
    if (xQueueReceive(xRs485Rec, &nrBloku, 1) != pdTRUE)
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
    if (xQueueReceive(xRs485Rec, &nrBloku, 1) != pdTRUE)
      blad = 13;
    else
    {
      if ((uint8_t)(crc & 0xFF) != nrBloku)
      blad = 14;
    }    
  }
  
  if ((blad != 0) && (blad != 253))
  {
    //odblokuj proces do cyklicznego komunikowania się z modułami wykonawczymi
    fprintf(&state->myStdInOut, "Blad nr %d\r\n", blad);    
    ramDyskZamknijPlik(&fdVty);
    czyscBufOdb485(state);  
    return;
  }
  if (blad == 0)
  {
    vTaskDelay(100);
    
    crc = _crc_xmodem_update(0, SYNC);            uartRs485SendByte(SYNC);
    crc = _crc_xmodem_update(crc, nrUrzadzenia);  uartRs485SendByte(nrUrzadzenia);
    crc = _crc_xmodem_update(crc, rFLASH);        uartRs485SendByte(rFLASH);
    crc = _crc_xmodem_update(crc, 1);             uartRs485SendByte(1);
    crc = _crc_xmodem_update(crc, nrUrzadzenia);  uartRs485SendByte(nrUrzadzenia);
    uartRs485SendByte((uint8_t)(crc>>8));
    uartRs485SendByte((uint8_t)(crc & 0xFF));

    //Odbieranie odpowiedzi od Bootloadera
    if(xQueueReceive(xRs485Rec, &data, 150) != pdTRUE)
    {
      blad = 15;
      fprintf(&state->myStdInOut, "Brak odpowiedzi Bootloadera\r\n");
    }
    if (data != 'C')
      blad = 16;
  }
  else
    blad = 0;
  
  if (blad != 0)
  {
    ramDyskZamknijPlik(&fdVty);
    //odblokuj proces do cyklicznego komunikowania się z modułami wykonawczymi
    return;
  } 

  uint8_t liczbaBlokow = fdVty.wpis->rozmiarHi * 2;
  if (fdVty.wpis->rozmiarLo == 128)
    liczbaBlokow++;
  fprintf(&state->myStdInOut, "Otrzymano odpowiedź od urządzenia. Liczba blokow do wyslania: %d\r\n", liczbaBlokow);
  
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
      if (ramDyskCzytajBajtZPliku(&fdVty, &data) != 0)
	data = 0;
      crc = _crc_xmodem_update(crc, data);
      uartRs485SendByte(data);
    }
    uartRs485SendByte((uint8_t)(crc>>8));
    uartRs485SendByte((uint8_t)(crc & 0xFF));

    if(xQueueReceive(xRs485Rec, &data, 100) != pdTRUE)
    {
      blad = 250;
      fprintf(&state->myStdInOut, "Brak odpowiedzi Bootloadera na ramke nr %d\r\n", nrBloku);
      break;
    }
    
    blad = czyscBufOdb485(&state->myStdInOut);
    if (blad != 0)
    {
      fprintf(&state->myStdInOut, "W buforze odbiorczym pozostalo %d znakow\r\n", blad);
      blad = 0;
    }
    if (data == ACK)
    {
      nrBloku ++;
      lRetransmisji = 0;
      fputc('+', &state->myStdInOut);
      continue;
    }

    if (data == CAN)
    {
      blad = 249;
      fprintf(&state->myStdInOut, "Odbiorca przerwal odbior\r\n");
      break;      
    }

    if (data == NAK)
    {
      fputc('-', &state->myStdInOut);
    }
    else
    {
      fputc('?', &state->myStdInOut);      
    }
    lRetransmisji ++;
    
    if (lRetransmisji == 3)
    {
      blad = 249;
      fprintf(&state->myStdInOut, "Przekroczono limit retransmisji\r\n");
      break;
    }
  }

  if (blad == 0)
  {
    uartRs485SendByte(EOT);

    if(xQueueReceive(xRs485Rec, &data, 25) == pdTRUE)
    {
      if (data == ACK)
      {
        uartRs485SendByte(EOT);
        xQueueReceive(xRs485Rec, &data, 25);
      }
    }
  }
  czyscBufOdb485(state);  
  ramDyskZamknijPlik(&fdVty);
  //odblokuj proces do cyklicznego komunikowania się z modułami wykonawczymi
  return;
}

static void goXmodemWyslijFunction(cmdState_t *state)
{
  fprintf(&state->myStdInOut, "Xmodem: rozpoczynanie wysylania\r\n");
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf(&state->myStdInOut, "Nie można otworzyć pliku\r\n");
    return;
  }
}

static void goXmodemOdbierzFunction(cmdState_t *state)
{
  fprintf(&state->myStdInOut, "Xmodem: rozpoczynanie odbioru\r\n");
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf(&state->myStdInOut, "Nie można otworzyć pliku\r\n");
    return;
  }
   
  uint8_t  i = 25;

  uint8_t  temp1;
//  uint8_t  temp2;

  uint8_t  c;
  uint8_t  liczbaProb;
  uint8_t  *zapPtr;
  uint8_t  *zapPtrKopia;
 
  uint16_t crcLokalne;
  uint8_t nrBloku;

  uint8_t nrBlokuZdalny;
  uint8_t nrBlokuZdalnyNeg;

  uint8_t crcHi;
  uint8_t crcLo;

  err1=0;
  err2=0;
  liczbaProb = 20;
  for ( ; ; )
  {
    fputc('C'              , &state->myStdInOut);
    while(!(UCSR1A & (1 << TXC1)));                              //CZekanie na opróżnienie bufora

    if(xQueueReceive(xVtyRec, &c, 100))
      if (c == SOH)
        break;                                                   //Rozpoczynamy transmisje

    liczbaProb--;
    if (liczbaProb == 0)
    {
      ramDyskZamknijPlik(&fdVty);
      cmdErrno = 255;
      return;
    }
  }
  
  nrBloku = 1;
  liczbaProb = 10;

  zapPtr          = ramDyskDodajBlokXmodem(&fdVty, nrBloku);
  zapPtrKopia     = zapPtr;
  for ( ; ; )
  {
    if (!xQueueReceive(xVtyRec, &nrBlokuZdalny, 100))
    {
      cmdErrno = 11;
      break; 
    }
    
    if (!xQueueReceive(xVtyRec, &nrBlokuZdalnyNeg, 1))
    {
      cmdErrno = 12;
      break; 
    }
  
    //1 Sprawdzanie, czy pasuje numer bloku z numerem bloku w usupełnieniu bitowym do 1
    c = 255-nrBlokuZdalnyNeg;
    if (nrBlokuZdalny != c)
    {
      cmdErrno = 1;
      err1 = nrBlokuZdalny;
      err2 = nrBlokuZdalnyNeg;
      break;
    }
    
    //Sprawdzenie, czy nie jest wznowiona transmisja poprzedniego bloku lub nie zaczęła się od bloku 0
    c = nrBloku-1;
    if (nrBlokuZdalny == c)
    {
      nrBloku = c;    //Cofnięcie nr aktualnego bloku o 1
      zapPtr = ramDyskDodajBlokXmodem(&fdVty, nrBloku);
      zapPtrKopia = zapPtr;
    }
    
    //2 Sprawdzanie, czy pasuje numer bloku
    if (nrBlokuZdalny != nrBloku)
    {
      cmdErrno = 2;
      err1 = nrBlokuZdalnyNeg;
      err2 = nrBloku;
      break;
    }
        
    for (i=0; i < XMODEM_BUFFER_SIZE; i++)
    {
      if(xQueueReceive(xVtyRec, &c, 10))
        *(zapPtr++) = c;
      else
      {
        cmdErrno = 3;
        break;
      }
    }
    if (!xQueueReceive(xVtyRec, &crcHi, 10))
    {
        cmdErrno = 13;
        break;      
    }
    if (!xQueueReceive(xVtyRec, &crcLo, 10))
    {
        cmdErrno = 13;
        break;      
    }

    //3 Zerowanie CRC
    crcLokalne=0; 
    
    //4 Obliczanie CRC
    for (i=0; i < XMODEM_BUFFER_SIZE; i++)
      crcLokalne = _crc_xmodem_update(crcLokalne, *(zapPtrKopia++));
     
    //5 Srawdzanie CRC
    if ((crcHi == crcLokalne / 256) && (crcLo == crcLokalne % 256))
    {
      liczbaProb = 10;
      uartVtySendByte(ACK);      
    }
    else
    {
      liczbaProb--;
      nrBloku--;
      uartVtySendByte(NAK);   
    }
    
    if (liczbaProb == 0)
    {
      err1 = nrBlokuZdalny;
      err2 = nrBloku;
      cmdErrno = 4;
      break; 
    }

    if (!xQueueReceive(xVtyRec, &temp1, 100))
    {
      cmdErrno = 10;
      break;
    }
    
    if (temp1 == SOH)
    {
      nrBloku++;
      zapPtr = ramDyskDodajBlokXmodem(&fdVty, nrBloku);
      zapPtrKopia = zapPtr;
      continue;
    }

    if (temp1 == CAN)
    {
      err2 = nrBloku;
      cmdErrno = 5;
      break;
    }
    if (temp1 == EOT)
    {
      uartVtySendByte(NAK);
      if (xQueueReceive(xVtyRec, &temp1, 10))
      {
        if (temp1 == EOT)
          uartVtySendByte(ACK);  
      }
      cmdErrno = 0;
      break;
    }
    cmdErrno = 7;
    break;
  }
  ramDyskZamknijPlik(&fdVty);
  return;
}

static void kasujRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  if ((rezultat =   ramDyskUsunPlik(cmdlineGetArgStr(1, state))) == 0)
    fprintf(&state->myStdInOut, "OK\r\n");
  else
    fprintf(&state->myStdInOut, "Blad: %d\r\n", rezultat);
}

static void dodajRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  if ((rezultat = ramDyskUtworzPlik(cmdlineGetArgStr(1, state))) == 0)
    fprintf(&state->myStdInOut, "OK\r\n");
  else
    fprintf(&state->myStdInOut, "Blad: %d\r\n", rezultat);
}

static void wypiszPlikiFunction(cmdState_t *state)
{
  ramDyskDir(&state->myStdInOut);
}

static void edytujRamPlikFunction(cmdState_t *state)
{
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf(&state->myStdInOut, "Nie można otworzyć pliku\r\n");
    return;
  }
  ramDyskUstawWskaznikNaKoniec(&fdVty);
  uint8_t znak = 0;
  fprintf(&state->myStdInOut, "Zapis do pliku. CTRL+C koniec\r\n");
  while(1)
  {
    if(!xQueueReceive( xVtyRec, &znak, portMAX_DELAY))
      continue;

    if (znak == 0x03)                                       // ^C
      break;

    uartVtySendByte(znak);                                  //Echo
    ramDyskZapiszBajtDoPliku(&fdVty, znak);
  }
  ramDyskZamknijPlik(&fdVty);
}

static void czytajRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  uint8_t znak = ' ';
  if ((rezultat = ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty)) != 0)
  {
    fprintf(&state->myStdInOut, "Brak pliku\r\n");
    return;
  }
  uint16_t rozmiar = fdVty.wpis->rozmiarHi * 256 + fdVty.wpis->rozmiarLo;
  fprintf(&state->myStdInOut, "Dlugosc pliku: %d\r\n", rozmiar);
  while (rezultat == 0)
  {
    rezultat = ramDyskCzytajBajtZPliku(&fdVty, &znak);
    
    uartVtySendByte(znak);
    if (znak == '\r')
      uartVtySendByte('\n');
  }
  fprintf(&state->myStdInOut, "\r\n");
  ramDyskZamknijPlik(&fdVty);
}

#ifdef testZewPamiec
static void testPamZewFunction(cmdState_t *state)
{
  state = NULL;
  uint8_t *ptr;
  ptr= 0x4000;
  uint8_t tmp;
  for (tmp=0; tmp <255; tmp++)
  {
    *(ptr) = tmp;
    ptr++;
  }
  ptr= 0x4000;
  uint8_t tmp2;
  for (tmp=0; tmp <4; tmp++)
  {
    uartVtySendByte('\r');
    uartVtySendByte('\n');
    for (tmp2=0; tmp2<64; tmp2++)
    {  
      uartVtySendByte(*(ptr));
      ptr++;
    }
  }
}
#endif

#ifdef debugRamDysk
char tmpChar[20];
static void testujRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  if ((rezultat = ramDyskUtworzPlik(cmdlineGetArgStr(1, state))) == 0)
    fprintf(&state->myStdInOut, "Utorzono plik\r\n");
  else
  {
    fprintf(&state->myStdInOut, "Blad tworzenia pliku: %d\r\n", rezultat);
    return;
  }
  
  if ((rezultat = ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty)) == 0)
    fprintf(&state->myStdInOut, "Otwarto plik (pierwszy klaster=%d)\r\n", fdVty.wpis->pierwszyKlaster);
  else
  {
    fprintf(&state->myStdInOut, "Blad Otwierania pliku: %d\r\n", rezultat);
    return;
  }
  
  if ((rezultat = ramDyskZapiszBajtDoPliku(&fdVty, 'a')) == 0)
    fprintf(&state->myStdInOut, "Zapisano znak do pliku\r\n");
  else
  {
    fprintf(&state->myStdInOut, "Blad zapisu znaku do pliku %d\r\n", rezultat);
    return;
  }
  
  rezultat = ramDyskZapiszBajtDoPliku(&fdVty, 'b');
  if (rezultat == 0)
    fprintf(&state->myStdInOut, "Zapisano znak do pliku\r\n");
  else
  {
    fprintf(&state->myStdInOut, "Blad zapisu znaku do pliku %d\r\n", rezultat);
    return;
  }
  uint8_t znak;
  
  rezultat = ramDyskUstawWskaznik(&fdVty, 0);
  if (rezultat == 0)
    fprintf(&state->myStdInOut, "Przestawiono wskaznik na poczatek pliku\r\n");
  else
  {
    fprintf(&state->myStdInOut, "Blad przy przestawianiu wskaznika %d\r\n", rezultat);
    return;
  }
  
  rezultat = ramDyskCzytajBajtZPliku(&fdVty, &znak);
  if (rezultat == 0)
    fprintf(&state->myStdInOut, "Odczytano znak %c\r\n", znak);
  else
  {
    fprintf(&state->myStdInOut, "Blad przy odczycie znaku %d\r\n", rezultat);
    return;
  }  
  rezultat = ramDyskCzytajBajtZPliku(&fdVty, &znak);
  if (rezultat == 0)
    fprintf(&state->myStdInOut, "Odczytano znak %c\r\n", znak);
  else
  {
    fprintf(&state->myStdInOut, "Blad przy odczycie znaku %d\r\n", rezultat);
    return;
  }
  ramDyskZamknijPlik(&fdVty);
}
#endif
