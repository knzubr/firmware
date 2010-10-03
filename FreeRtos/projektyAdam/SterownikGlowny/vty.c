#include "main.h"
#include "vty.h"
#include "ramdysk.h"
#include "protRs485.h"
#include "protocol1.h"
#include "mpc23s17.h"
#include "mcp3008.h"
#include "ds1305.h"
#include "hardwareConfig.h"
#include "configuration.h"

#include "vty_en.h"
//#include "vty_pl.h"

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif


static cliExRes_t helpFunction           (cmdState_t *state);
static cliExRes_t statusFunction         (cmdState_t *state);
static cliExRes_t opuscFunction          (cmdState_t *state);
static cliExRes_t podniesFunction        (cmdState_t *state);
static cliExRes_t pingFunction           (cmdState_t *state);
static cliExRes_t goXmodemOdbierzFunction(cmdState_t *state);
static cliExRes_t goXmodemWyslijFunction (cmdState_t *state);
static cliExRes_t dodajRamPlikFunction   (cmdState_t *state);
static cliExRes_t kasujRamPlikFunction   (cmdState_t *state);
static cliExRes_t flashowanieModWyk      (cmdState_t *state);
static cliExRes_t wypiszPlikiFunction    (cmdState_t *state);
static cliExRes_t edytujRamPlikFunction  (cmdState_t *state);
static cliExRes_t czytajRamPlikFunction  (cmdState_t *state);

static cliExRes_t ustawPortExtAFunction  (cmdState_t *state);

static cliExRes_t pokazCzasFunction      (cmdState_t *state);
static cliExRes_t czytajAC_Function      (cmdState_t *state);

static cliExRes_t enableFunction         (cmdState_t *state);
static cliExRes_t disableFunction        (cmdState_t *state);
static cliExRes_t configureModeFunction  (cmdState_t *state);

static cliExRes_t ustawIpFunction        (cmdState_t *state);
static cliExRes_t setMacAddrFunction     (cmdState_t *state);
static cliExRes_t ustawCzasFunction      (cmdState_t *state);

static cliExRes_t saveConfigFunction     (cmdState_t *state);

#ifdef testZewPamiec
static cliExRes_t testPamZewFunction     (cmdState_t *state);
#endif

struct ramPlikFd    fdVty;  //TODO move it to CLI struct

prog_char okStr[] = "OK\r\n";
prog_char nlStr[] = "\r\n";
prog_char BladBuforaPozostaloBajtowStr[]           = "!!! W budorze Rs485 pozostalo %d bajtow\r\n";


prog_char __ATTR_PROGMEM__ *errorStrings[] = {
  errorOK,
  errorNoFile,
  errorxModemFrameStartTimeout,
  errorxModemByteSendTimeout,
  errorxModemWrongFrameNo,
  errorxModemFrameFrameNoCorrectionNotMatch,
  errorxModemFrameCrc,
  errorxModemRemoteSideCan,
  errorxModemUnknownResponse,
  errorBootloaderNotResponding
};

command_t __ATTR_PROGMEM__ cmdListNormal[] =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_time,      cmd_help_time,      pokazCzasFunction},  
  {cmd_ping,      cmd_help_ping,      pingFunction},
  {cmd_dir_rf,    cmd_help_dir_rf,    wypiszPlikiFunction},
  {cmd_read_rf,   cmd_help_read_rf,   czytajRamPlikFunction},
  {cmd_enable,    cmd_help_enable,    enableFunction},
  {NULL, NULL, NULL}
};

command_t __ATTR_PROGMEM__ cmdListEnable[] =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_time,      cmd_help_time,      pokazCzasFunction},
  
  {cmd_ping,      cmd_help_ping,      pingFunction},
  {cmd_xRec,      cmd_help_xRec,      goXmodemOdbierzFunction},
  {cmd_xSend,     cmd_help_xSend,     goXmodemWyslijFunction},
  {cmd_xflash,    cmd_help_xflash,    flashowanieModWyk},
#ifdef testZewPamiec
  {cmd_rtest,     cmd_help_rtest,     testPamZewFunction},
#endif
  {cmd_dir_rf,    cmd_help_dir_rf,    wypiszPlikiFunction},
  {cmd_create_rf, cmd_help_create_rf, dodajRamPlikFunction},
  {cmd_erase_rf,  cmd_help_erase_rf,  kasujRamPlikFunction},
  {cmd_edit_rf,   cmd_help_edit_rf,   edytujRamPlikFunction},
  {cmd_read_rf,   cmd_help_read_rf,   czytajRamPlikFunction},

  {cmd_up,        cmd_help_up,        podniesFunction},
  {cmd_down,      cmd_help_down,      opuscFunction},

  {cmd_spa,       cmd_help_spa,       ustawPortExtAFunction},
  {cmd_settime,   cmd_help_settime,   ustawCzasFunction},
  {cmd_ac,        cmd_help_ac,        czytajAC_Function},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {NULL, NULL, NULL}
};

command_t __ATTR_PROGMEM__ cmdListConfigure[] =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_time,      cmd_help_time,      pokazCzasFunction},
  
  {cmd_settime,   cmd_help_settime,   ustawCzasFunction},
  {cmd_conf_ip,   cmd_help_conf_ip,   ustawIpFunction},
  {cmd_conf_mac,  cmd_help_conf_mac,  setMacAddrFunction},
  {cmd_conf_save, cmd_help_conf_save, saveConfigFunction},

  {cmd_enable,    cmd_help_enable,    enableFunction},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {NULL, NULL, NULL}
};

void VtyInit(cmdState_t* state)
{
  cmdStateConfigure(state, (char *)(CLI_1_BUF_ADDR), CLI_BUF_TOT_LEN, VtyPutChar, &cmdListNormal[0], NR_NORMAL);
}

void printErrorInfo(cmdState_t *state)
{
  if (state->errno != 0)
    fprintf_P(&state->myStdInOut, errorStrings[state->errno], state->err1, state->err2);
}

static cliExRes_t enableFunction(cmdState_t *state)
{
  if (state->cliMode != RESTRICTED_NORMAL)
  {
    state->cmdList = cmdListEnable;
    state->cliMode = NR_ENABLE;
    return OK_SILENT;
  }
  return ERROR_OPERATION_NOT_ALLOWED;
}
static cliExRes_t disableFunction(cmdState_t *state)
{
  state->cmdList = cmdListNormal;
  if (state->cliMode != RESTRICTED_NORMAL)
  {
    state->cliMode = NR_NORMAL;
  }
  return OK_SILENT;
}

static cliExRes_t configureModeFunction(cmdState_t *state)
{
  if (state->cliMode == NR_ENABLE)
  {
    state->cmdList = cmdListConfigure;
    state->cliMode = NR_CONFIGURE;
    return OK_SILENT;
  }
  return ERROR_OPERATION_NOT_ALLOWED;
}

// ******************************************************************************************************************************************************

static cliExRes_t statusFunction(cmdState_t *state)
{
  fprintf_P(&state->myStdInOut, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n")); 
  //Print system state
  fprintf_P(&state->myStdInOut, systemStateStr);
  fprintf_P(&state->myStdInOut, statusNumberOfTasksStr,    uxTaskGetNumberOfTasks());
  fprintf_P(&state->myStdInOut, statusStaticHeapStateStr,  xPortGetFreeHeapSize(), configTOTAL_HEAP_SIZE);
  fprintf_P(&state->myStdInOut, statusDynamicHeapStateStr, xmallocAvailable(),   HEAP_SIZE);  //TODO FIXME
  uint8_t ramDiscSpace = ramDyskLiczbaWolnychKlastrow();
  fprintf_P(&state->myStdInOut, statusRamDiskStateStr,     ramDiscSpace,  L_KLASTROW);
  printErrorInfo(state);
  
  //Print system configuration
  fprintf_P(&state->myStdInOut, systemRamConfigStr);
  fprintf_P(&state->myStdInOut, statusMacStr, mymac[0], mymac[1], mymac[2], mymac[3], mymac[4], mymac[5]);
  fprintf_P(&state->myStdInOut, statusIpStr, myip[0], myip[1], myip[2], myip[3], mask);
  
  //Print time
/*  readTimeDecoded((timeDecoded_t *)(&czasRtc));
  uint8_t godzina = 10*czasRtc.hours.syst24.cDzies + czasRtc.hours.syst24.cJedn;  
  uint8_t minuta =  10*czasRtc.minutes.cDzies + czasRtc.minutes.cJedn;
  uint8_t sekunda = 10*czasRtc.seconds.cDzies + czasRtc.seconds.cJedn;
  fprintf_P(&state->myStdInOut, PSTR("%d:%d:%d\r\n"), godzina, minuta, sekunda);*/
  return OK_SILENT;
}

static cliExRes_t pokazCzasFunction(cmdState_t *state)
{
  readTimeDecoded((timeDecoded_t *)(&czasRtc));
  uint8_t godzina = 10*czasRtc.hours.syst24.cDzies + czasRtc.hours.syst24.cJedn;  
  uint8_t minuta =  10*czasRtc.minutes.cDzies + czasRtc.minutes.cJedn;
  uint8_t sekunda = 10*czasRtc.seconds.cDzies + czasRtc.seconds.cJedn;  
  fprintf_P(&state->myStdInOut, PSTR("Aktualny czas %d:%d:%d\r\n"), godzina, minuta, sekunda);
  return OK_SILENT;
}

static cliExRes_t ustawCzasFunction(cmdState_t *state)
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
  
  setTimeDecoded((timeDecoded_t *)(&czasRtc));
  return OK_SILENT;
}

static cliExRes_t ustawIpFunction(cmdState_t *state)
{
  if (state->argc < 5)
    return SYNTAX_ERROR;
  myip[0] =   cmdlineGetArgInt(1, state);
  myip[1] =   cmdlineGetArgInt(2, state);
  myip[2] =   cmdlineGetArgInt(3, state);
  myip[3] =   cmdlineGetArgInt(4, state);
  mask    =   cmdlineGetArgInt(5, state);
  return OK_SILENT;
}

static cliExRes_t setMacAddrFunction(cmdState_t *state)
{
  if (state->argc < 6)
    return SYNTAX_ERROR;  mymac[0] = cmdlineGetArgHex(1, state);

  mymac[1] = cmdlineGetArgHex(2, state);
  mymac[2] = cmdlineGetArgHex(3, state);
  mymac[3] = cmdlineGetArgHex(4, state);
  mymac[4] = cmdlineGetArgHex(5, state);
  mymac[5] = cmdlineGetArgHex(6, state);
  return OK_SILENT;
}

static cliExRes_t czytajAC_Function(cmdState_t *state)
{
  uint8_t nrWejscia = cmdlineGetArgInt(1, state);
  uint16_t wynik = MCP3008_getSampleSingle(nrWejscia);
  fprintf_P(&state->myStdInOut, PSTR("Wartosc probki na wejsciu %d: %d\r\n"), nrWejscia, wynik);  
  return OK_SILENT;
}

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}


prog_char OpuszczanieRoletyStr[] =
"Opuszczanie rolety\r\n"
"\tsterownik %d\r\n"
"\troleta    %d\r\n";
static cliExRes_t opuscFunction(cmdState_t *state)
{
  uint8_t nrRolety;
  uint8_t nrSterownika;
  uint8_t wartosc;
  
  nrSterownika = cmdlineGetArgInt(1, state);
  nrRolety = cmdlineGetArgInt(2, state);
  nrRolety &= 0x01;
  wartosc = cmdlineGetArgInt(3, state);

  fprintf_P(&state->myStdInOut,OpuszczanieRoletyStr, nrSterownika, nrRolety+1);
  if ((wartosc > 0) && (wartosc <=100))
    fprintf_P(&state->myStdInOut, PSTR("\tpozycja   %d\r\n"), wartosc);

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
  return OK_SILENT;
}

prog_char PodnoszenieRoletyStr[] =
"Podnoszenie rolety\r\n"
"\tsterownik %d\r\n"
"\troleta    %d\r\n";
static cliExRes_t podniesFunction(cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;
  
  uint8_t nrSterownika = (cmdlineGetArgInt(1, state) & 0x3F);
  uint8_t nrRolety     = (cmdlineGetArgInt(2, state) & 0x01);
  uint8_t wartosc = 255;
  if (state->argc > 2)
    wartosc = cmdlineGetArgInt(3, state);

  fprintf_P(&state->myStdInOut,   PodnoszenieRoletyStr, nrSterownika, nrRolety+1);
  if ((wartosc > 0) && (wartosc <=100))
    fprintf_P(&state->myStdInOut, PSTR("\tpozycja   %d\r\n"), wartosc);

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
  return OK_SILENT;
}

static cliExRes_t ustawPortExtAFunction(cmdState_t *state)
{
  uint8_t wyjscie = cmdlineGetArgInt(1, state);
  MPC23s17SetDirA(0x00, 0);
  MPC23s17SetPortA(wyjscie, 0);
  return OK_SILENT;
}

prog_char pingNieOdpowiada[]    = "Urzedzenie nr %d nie odpowiada\r\n";
prog_char pingBladOdpowiedzi[]  = "Blad nr %d w odpowiedzi urzadzenia nr %d\r\n";
prog_char pingZnalezionoStr[]   = "Znaleziono urzadzanie nr %d\r\n";

static cliExRes_t pingFunction(cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;
  
  static uint8_t nrSterownika;
  static uint8_t wynik;
  static uint8_t l_znakow;

  nrSterownika = (uint8_t)(cmdlineGetArgInt(1, state));
  l_znakow = cmdlineGetArgInt(2, state);

  wynik = czyscBufOdb485(NULL);
  if (wynik != 0)
    fprintf_P(&state->myStdInOut, BladBuforaPozostaloBajtowStr, wynik);
    
  sendPing(nrSterownika, l_znakow, uartRs485SendByte);
//  czyscBufOdb485(state);  
  wynik = sprOdebrPing(nrSterownika, l_znakow, state);
  if (wynik != 0)
  {
    czyscBufOdb485(state);
    if (wynik == 1)
      fprintf_P(&state->myStdInOut, pingNieOdpowiada, nrSterownika);
    else
      fprintf_P(&state->myStdInOut, pingBladOdpowiedzi, wynik, nrSterownika);
  }
  else
    fprintf_P(&state->myStdInOut, pingZnalezionoStr, nrSterownika);
  return OK_SILENT;
}

prog_char BladOtwarciePliku[]                  = "Nie mozna otworzyc pliku %s\r\n";
prog_char flashowanieBrakOdpBootloaderaStr[]   = "Brak odpowiedzi bootloadera\r\n";
prog_char flashowanieOczekiwanieNaBootloader[] = "Wykonuje się kod bootloadera, który oczekuje startu transmisji\r\n";
prog_char flashowanieOtrzymanoOdpStr[]         = "Otrzymano odpowiedź od urządzenia. Liczba blokow do wyslania: %d\r\n";
prog_char flashowanieBrakOdpNaRamkeNrStr[]     = "Brak odpowiedzi na ramke nr %d\r\n";
prog_char flashowaniePozostaloWBufZnakowStr[]  = "W buforze odbiorczym pozostalo %d znakow\r\n";
prog_char flashowanieOdbPrzerwOdbStr[]         = "Odbiorca przerwal odbior\r\n";
prog_char flashowaniePrzekroczonoLimitStr[]    = "Przekroczono limit retransmisji\r\n";
static cliExRes_t flashowanieModWyk(cmdState_t *state)
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
    fprintf_P(&state->myStdInOut, BladBuforaPozostaloBajtowStr, blad);
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
    fprintf_P(&state->myStdInOut, BladOtwarciePliku, nazwaPliku);
    return ERROR_INFORM;
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
    fprintf_P(&state->myStdInOut, flashowanieOczekiwanieNaBootloader);
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
    printErrorInfo(state);    
    ramDyskZamknijPlik(&fdVty);
    czyscBufOdb485(state);  
    return ERROR_INFORM;
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
      fprintf_P(&state->myStdInOut, flashowanieBrakOdpBootloaderaStr);
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
    return ERROR_INFORM;
  } 

  uint8_t liczbaBlokow = fdVty.wpis->rozmiarHi * 2;
  if (fdVty.wpis->rozmiarLo == 128)
    liczbaBlokow++;
  fprintf_P(&state->myStdInOut, flashowanieOtrzymanoOdpStr, liczbaBlokow);
  
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
      fprintf_P(&state->myStdInOut, flashowanieBrakOdpNaRamkeNrStr, nrBloku);
      break;
    }
    
    blad = czyscBufOdb485(state);
    if (blad != 0)
    {
      fprintf_P(&state->myStdInOut, flashowaniePozostaloWBufZnakowStr, blad);
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
      fprintf_P(&state->myStdInOut, flashowanieOdbPrzerwOdbStr);
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
      fprintf_P(&state->myStdInOut, flashowaniePrzekroczonoLimitStr);
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
  return OK_SILENT;
}

prog_char xwyslijStartStr[] = "Xmodem: rozpoczynanie wysylania\r\n";
static cliExRes_t goXmodemWyslijFunction(cmdState_t *state)
{
  fprintf_P(&state->myStdInOut, xwyslijStartStr);
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf_P(&state->myStdInOut, BladOtwarciePliku, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
  }
  return OK_SILENT;
}

static cliExRes_t goXmodemOdbierzFunction(cmdState_t *state)
{
  fprintf_P(&state->myStdInOut, PSTR("Xmodem: rozpoczynanie odbioru\r\n"));
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf_P(&state->myStdInOut, BladOtwarciePliku, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
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

  state->err1=0;
  state->err2=0;
  liczbaProb = 20;
  for ( ; ; )
  {
    fputc('C'              , &state->myStdInOut);
    while(!(UCSR1A & (1 << TXC1)));                              //Czekanie na opróżnienie bufora

    if(xQueueReceive(xVtyRec, &c, 100))
      if (c == SOH)
        break;                                                   //Rozpoczynamy transmisje

    liczbaProb--;
    if (liczbaProb == 0)
    {
      ramDyskZamknijPlik(&fdVty);
      state->errno = (uint8_t)(AllOK);
      return ERROR_INFORM;
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
      state->errno = (uint8_t)(xModemFrameStartTimeout);
      break; 
    }
    
    if (!xQueueReceive(xVtyRec, &nrBlokuZdalnyNeg, 1))
    {
      state->errno = (uint8_t)(xModemByteSendTimeout);
      break; 
    }
  
    //1 Sprawdzanie, czy pasuje numer bloku z numerem bloku w usupełnieniu bitowym do 1
    c = 255-nrBlokuZdalnyNeg;
    if (nrBlokuZdalny != c)
    {
      state->errno = (uint8_t)(xModemFrameFrameNoCorrectionNotMatch);
      state->err1 = nrBlokuZdalny;
      state->err2 = nrBlokuZdalnyNeg;
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
      state->errno = (uint8_t)(xModemWrongFrameNo);
      state->err1 = nrBlokuZdalnyNeg;
      state->err2 = nrBloku;
      break;
    }
        
    for (i=0; i < XMODEM_BUFFER_SIZE; i++)
    {
      if(xQueueReceive(xVtyRec, &c, 10))
        *(zapPtr++) = c;
      else
      {
        state->errno = (uint8_t)(xModemByteSendTimeout);
        break;
      }
    }
    if (!xQueueReceive(xVtyRec, &crcHi, 10))
    {
        state->errno = (uint8_t)(xModemFrameCrc);
        state->err1 = 2;
        break;      
    }
    if (!xQueueReceive(xVtyRec, &crcLo, 10))
    {
        state->errno = (uint8_t)(xModemFrameCrc);
        state->err1 = 1;
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
      state->err1 = nrBlokuZdalny;
      state->err2 = nrBloku;
      state->errno = (uint8_t)(xModemWrongFrameNo);
      break; 
    }

    if (!xQueueReceive(xVtyRec, &temp1, 100))
    {
      state->errno = (uint8_t)(xModemFrameStartTimeout);
      break;
    }
    
    if (temp1 == SOH)
    {
      nrBloku++;
      zapPtr = ramDyskDodajBlokXmodem(&fdVty, nrBloku);
      zapPtrKopia = zapPtr;
      state->errno = (uint8_t)(AllOK);
      continue;
    }

    if (temp1 == CAN)
    {
      state->err1 = nrBloku;
      state->errno = (uint8_t)(xModemRemoteSideCan);
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
      state->errno = (uint8_t)(AllOK);
      break;
    }
    state->errno = (uint8_t)(xModemUnknownResponse);
    state->err1 = temp1;
    break;
  }
  ramDyskZamknijPlik(&fdVty);
  return OK_SILENT;
}

static cliExRes_t kasujRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  if ((rezultat =   ramDyskUsunPlik(cmdlineGetArgStr(1, state))) == 0)
    fprintf_P(&state->myStdInOut, okStr);
  printErrorInfo(state);
  return OK_SILENT;
}

static cliExRes_t dodajRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  if ((rezultat = ramDyskUtworzPlik(cmdlineGetArgStr(1, state))) == 0)
  {
    fprintf_P(&state->myStdInOut, okStr);
    return OK_INFORM;
  }
  printErrorInfo(state);
  return OK_INFORM;
}

static cliExRes_t wypiszPlikiFunction(cmdState_t *state)
{
  ramDyskDir(&state->myStdInOut);
  return OK_SILENT;
}

prog_char zapisDoPlikuStr[] = "Zapis do pliku. CTRL+C koniec\r\n";
static cliExRes_t edytujRamPlikFunction(cmdState_t *state)
{
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf_P(&state->myStdInOut, BladOtwarciePliku, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
  }
  ramDyskUstawWskaznikNaKoniec(&fdVty);
  uint8_t znak = 0;
  fprintf_P(&state->myStdInOut, zapisDoPlikuStr);
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
  return OK_SILENT;
}

prog_char dulgoscPlikuStr[] = "Dlugosc pliku: %d\r\n";

static cliExRes_t czytajRamPlikFunction(cmdState_t *state)
{
  uint8_t rezultat;
  uint8_t znak = ' ';
  if ((rezultat = ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty)) != 0)
  {
    fprintf_P(&state->myStdInOut, BladOtwarciePliku, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
  }
  uint16_t rozmiar = fdVty.wpis->rozmiarHi * 256 + fdVty.wpis->rozmiarLo;
  fprintf_P(&state->myStdInOut,dulgoscPlikuStr , rozmiar);
  while (rezultat == 0)
  {
    rezultat = ramDyskCzytajBajtZPliku(&fdVty, &znak);
    
    uartVtySendByte(znak);
    if (znak == '\r')
      uartVtySendByte('\n');
  }
  fprintf_P(&state->myStdInOut, nlStr);
  ramDyskZamknijPlik(&fdVty);
  return OK_SILENT;
}

static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  state = NULL;
  saveConfiguration();
  return OK_SILENT;
}

#ifdef testZewPamiec
static cliExRes_t testPamZewFunction(cmdState_t *state)
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
  return OK_SILENT;
}
#endif


