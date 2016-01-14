#include "main.h"
#include "vty.h"
#include "ramdysk.h"
#include "protocol1.h"
#include "mpc23s17.h"
#include "mcp3008.h"
#include "ds1305.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "Rs485_prot.h"
#include "net.h"
#include "ip.h"
#include "arp.h"
#include "softwareConfig.h"
#include "mcp4150.h"

#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif


static cliExRes_t helpFunction           (cmdState_t *state);
static cliExRes_t statusFunction         (cmdState_t *state);
static cliExRes_t statusEncFunction      (cmdState_t *state);
static cliExRes_t pingFunction           (cmdState_t *state);
static cliExRes_t dodajRamPlikFunction   (cmdState_t *state);
static cliExRes_t eraseRamFileFunction   (cmdState_t *state);
static cliExRes_t flashExModuleFunction  (cmdState_t *state);
static cliExRes_t writeRamFileFunction   (cmdState_t *state);
static cliExRes_t editRamFileFunction    (cmdState_t *state);
static cliExRes_t readRamFIleFunction    (cmdState_t *state);

static cliExRes_t pokazCzasFunction      (cmdState_t *state);
static cliExRes_t debugFunction          (cmdState_t *state);

static cliExRes_t enableFunction         (cmdState_t *state);
static cliExRes_t disableFunction        (cmdState_t *state);
static cliExRes_t configureModeFunction  (cmdState_t *state);

static cliExRes_t setIpFunction(cmdState_t *state);
static cliExRes_t setIpMaskFunction(cmdState_t *state);
static cliExRes_t setIpGwFunction(cmdState_t *state);
static cliExRes_t setUdpFunction(cmdState_t *state);

static cliExRes_t setMacAddrFunction     (cmdState_t *state);
static cliExRes_t setTimeFunction        (cmdState_t *state);

static cliExRes_t saveConfigFunction     (cmdState_t *state);

#ifdef testZewPamiec
static cliExRes_t testPamZewFunction     (cmdState_t *state);
#endif

struct ramPlikFd    fdVty;  //TODO move it to CLI struct

const char okStr[]                        PROGMEM = "OK\r\n";
const char nlStr[]                        PROGMEM = "\r\n";
const char BladBuforaPozostaloBajtowStr[] PROGMEM = "!!! W budorze Rs485 pozostalo %d bajtow\r\n";


const  char * const errorStrings[] PROGMEM = {
  errorOK,
  errorNoFile,
  errorxModemFrameStartTimeout,
  errorxModemByteSendTimeout,
  errorxModemWrongFrameNo,
  errorxModemFrameFrameNoCorrectionNotMatch,
  errorxModemFrameCrc,
  errorxModemRemoteSideCan,
  errorxModemUnknownResponse,
  errorNoRemoteDevice,
  errorBootloaderNotResponding,
  errorOpenFile
};

const command_t cmdListNormal[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_time,      cmd_help_time,      pokazCzasFunction},
  {cmd_ping,      cmd_help_ping,      pingFunction},
  {cmd_dir_rf,    cmd_help_dir_rf,    writeRamFileFunction},
  {cmd_read_rf,   cmd_help_read_rf,   readRamFIleFunction},
  {cmd_enable,    cmd_help_enable,    enableFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_enc_stat,  cmd_help_enc_stat,  statusEncFunction},
  {cmd_time,      cmd_help_time,      pokazCzasFunction},
  {cmd_net_dbg,   cmd_help_net_dbg,   debugFunction},

  {cmd_ping,      cmd_help_ping,      pingFunction},
#ifdef testZewPamiec
  {cmd_rtest,     cmd_help_rtest,     testPamZewFunction},
#endif
  {cmd_dir_rf,    cmd_help_dir_rf,    writeRamFileFunction},
  {cmd_create_rf, cmd_help_create_rf, dodajRamPlikFunction},
  {cmd_erase_rf,  cmd_help_erase_rf,  eraseRamFileFunction},
  {cmd_edit_rf,   cmd_help_edit_rf,   editRamFileFunction},
  {cmd_read_rf,   cmd_help_read_rf,   readRamFIleFunction},

  {cmd_settime,   cmd_help_settime,   setTimeFunction},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_time,         cmd_help_time,         pokazCzasFunction},
  {cmd_settime,      cmd_help_settime,      setTimeFunction},
  {cmd_conf_ip,      cmd_help_conf_ip,      setIpFunction},
  {cmd_conf_ip_mask, cmd_conf_ip_mask_help, setIpMaskFunction},
  {cmd_conf_ip_gw,   cmd_conf_ip_gw_help,   setIpGwFunction},
  {cmd_conf_udp,     cmd_help_conf_udp,     setUdpFunction},
  {cmd_conf_mac,     cmd_help_conf_mac,     setMacAddrFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_enable,       cmd_help_enable,       enableFunction},
  {cmd_disable,      cmd_help_disable,      disableFunction},
  {NULL, NULL, NULL}
};

void VtyInit(cmdState_t* state, FILE *stream)
{
  cmdStateConfigure(state, (char *)(CLI_1_BUF_ADDR), CLI_BUF_TOT_LEN, stream, &cmdListNormal[0], NR_NORMAL);
}

void printErrorInfo(cmdState_t *state)
{
  if (state->errno != 0)
  {
    fprintf_P(state->myStdInOut, (const char*)(pgm_read_word(errorStrings + state->errno)), state->err1, state->err2);
  }
  state->errno = 0;
  state->err1 = 0;
  state->err2 = 0;
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

// ************************** VTY API ***************************************************************************************
void printStatus(FILE *stream)
{
  fprintf_P(stream, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));
  //Print system state
  fprintf_P(stream, systemStateStr);
  fprintf_P(stream, statusNumberOfTasksStr,    uxTaskGetNumberOfTasks());
  fprintf_P(stream, statusStaticHeapStateStr,  xPortGetFreeHeapSize(), configTOTAL_HEAP_SIZE);
  fprintf_P(stream, statusDynamicHeapStateStr, xmallocAvailable(),   HEAP_SIZE);
  fprintf_P(stream, statusTemperatureStr, temperature);
  fprintf_P(stream, statusVoltageStr, voltage);

  uint8_t tmp = ramDyskLiczbaWolnychKlastrow();
  fprintf_P(stream, statusRamDiskStateStr,     tmp,  L_KLASTROW);
//  printErrorInfo(state); //TODO fix and uncomment

  //Print system configuration
  fprintf_P(stream, systemRamConfigStr);

  fprintf_P(stream, statusMacStr);
  netPrintEthAddr(stream, &nicState.mac);
  fprintf_P(stream, PSTR("\r\n"));

  fprintf_P(stream, statusIpStr);
  netPrintIPAddr(stream, ipGetConfig()->ip);
  fprintf_P(stream, PSTR("\r\n"));

  fprintf_P(stream, statusIpMaskStr);
  netPrintIPAddr(stream, ipGetConfig()->netmask);
  fprintf_P(stream, PSTR("\r\n"));

  fprintf_P(stream, statusIpGwStr);
  netPrintIPAddr(stream, ipGetConfig()->gateway);
  fprintf_P(stream, PSTR("\r\n"));

  //Print Rs485 Execitive modules
  fprintf_P(stream, statusRs485listStr);


  //Print locker sensors
  fprintf_P(stream, statusLockerSensorsStr);
  tmp = printLockers(stream);
  if (tmp == 0)
    fprintf_P(stream, statusLockerSensorsDisStr);

  //Print time FIXME deadlock problem
/*  readTimeDecoded((timeDecoded_t *)(&czasRtc));
  uint8_t godzina = 10*czasRtc.hours.syst24.cDzies + czasRtc.hours.syst24.cJedn;
  uint8_t minuta =  10*czasRtc.minutes.cDzies + czasRtc.minutes.cJedn;
  uint8_t sekunda = 10*czasRtc.seconds.cDzies + czasRtc.seconds.cJedn;
  fprintf_P(state->myStdInOut, PSTR("%d:%d:%d\r\n"), godzina, minuta, sekunda);*/

  udpPrintStatus(stream);
//  arpPrintTable(stream);
}


// ************************** CLI Functions *********************************************************************************

static cliExRes_t statusFunction(cmdState_t *state)
{
  if (state->argc < 1)
  {
    printStatus(state->myStdInOut);
    return OK_SILENT;
  }

  FILE stream;
  if (ramDyskOtworzPlikStdIo(cmdlineGetArgStr(1, state), &fdVty, &stream, __SWR | __SRD) != 0)
  {
    fprintf_P(state->myStdInOut, errorOpenFile, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
  }

  printStatus(&stream);
  ramDyskZamknijPlikStdIo(&stream);
  return OK_SILENT;
}

static cliExRes_t statusEncFunction(cmdState_t *state)
{
  nicRegDump(state->myStdInOut);
  return OK_SILENT;
}

static cliExRes_t pokazCzasFunction(cmdState_t *state)
{
  readTimeDecoded((timeDecoded_t *)(&czasRtc));
  uint8_t godzina = 10*czasRtc.hours.syst24.cDzies + czasRtc.hours.syst24.cJedn;
  uint8_t minuta =  10*czasRtc.minutes.cDzies + czasRtc.minutes.cJedn;
  uint8_t sekunda = 10*czasRtc.seconds.cDzies + czasRtc.seconds.cJedn;
  fprintf_P(state->myStdInOut, PSTR("Aktualny czas %d:%d:%d\r\n"), godzina, minuta, sekunda);
  return OK_SILENT;
}

static cliExRes_t debugFunction          (cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;

  uint8_t level = cmdlineGetArgInt(2, state);
  const char *str = (const char*)cmdlineGetArgStr(1, state);
  if (level == 0)
  {
    if (strncmp_P(str, PSTR("arp"), 3) == 0)
    {
      setArpDebug(NULL, 0);
      fprintf_P(state->myStdInOut, debugDisabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("ip"), 2) == 0)
    {
      setIpDebug(NULL, 0);
      fprintf_P(state->myStdInOut, debugDisabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("icmp"), 2) == 0)
    {
      setIcmpDebug(NULL, 0);
      fprintf_P(state->myStdInOut, debugDisabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("tcp"), 2) == 0)
    {
      setTcpDebug(NULL, 0);
      fprintf_P(state->myStdInOut, debugDisabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("udp"), 2) == 0)
    {
      setUdpDebug(NULL, 0);
      fprintf_P(state->myStdInOut, debugDisabledInfoStr, str);
      return OK_SILENT;
    }


  }
  else                   //level > 0
  {
    if (strncmp_P(str, PSTR("arp"), 3) == 0)
    {
      setArpDebug(state->myStdInOut, level);
      fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("ip"), 2) == 0)
    {
      setIpDebug(state->myStdInOut, level);
      fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("icmp"), 2) == 0)
    {
      setIcmpDebug(state->myStdInOut, level);
      fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("tcp"), 2) == 0)
    {
      setTcpDebug(state->myStdInOut, level);
      fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
      return OK_SILENT;
    }

    if (strncmp_P(str, PSTR("udp"), 2) == 0)
    {
      setUdpDebug(state->myStdInOut, level);
      fprintf_P(state->myStdInOut, debugEnabledInfoStr, str);
      return OK_SILENT;
    }
  }

  return SYNTAX_ERROR;
}


static cliExRes_t setTimeFunction(cmdState_t *state)
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

static cliExRes_t setIpFunction(cmdState_t *state)
{
  if (state->argc < 4)
    return SYNTAX_ERROR;

  uint32_t ip = cmdlineGetArgInt(1, state) +
                (((uint32_t)(cmdlineGetArgInt(2, state)))<< 8) +
                (((uint32_t)(cmdlineGetArgInt(3, state)))<<16) +
                (((uint32_t)(cmdlineGetArgInt(4, state)))<<24);

  ipSetConfigIp(ip);
  return OK_SILENT;
}

static cliExRes_t setUdpFunction(cmdState_t *state)
{
  if (state->argc < 5)
    return SYNTAX_ERROR;

  uint32_t ip = cmdlineGetArgInt(1, state) +
                (((uint32_t)(cmdlineGetArgInt(2, state)))<< 8) +
                (((uint32_t)(cmdlineGetArgInt(3, state)))<<16) +
                (((uint32_t)(cmdlineGetArgInt(4, state)))<<24);
  udpSocket->dstIp = ip;

  uint16_t port = cmdlineGetArgInt(5, state);
  udpSocket->srcPort = htons(port);

  if (state->argc > 5)
  {
    port = cmdlineGetArgInt(6, state);
    udpSocket->dstPort = htons(port);
  }
  return OK_SILENT;
}


static cliExRes_t setIpMaskFunction(cmdState_t *state)
{
  if (state->argc < 1)
    return SYNTAX_ERROR;

  uint32_t mask = ((uint32_t)(0xFFFFFFFF))>>(32-cmdlineGetArgInt(1, state));

  ipSetConfigMask(mask);
  return OK_SILENT;
}


static cliExRes_t setIpGwFunction(cmdState_t *state)
{
  if (state->argc < 4)
    return SYNTAX_ERROR;

  uint32_t gw = cmdlineGetArgInt(1, state) +
                (((uint32_t)(cmdlineGetArgInt(2, state)))<< 8) +
                (((uint32_t)(cmdlineGetArgInt(3, state)))<<16)  +
                (((uint32_t)(cmdlineGetArgInt(4, state)))<<24);
  ipSetConfigGw(gw);
  return OK_SILENT;
}

static cliExRes_t ustawModWykFunction(cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;

  uint8_t adres =   cmdlineGetArgInt(1, state);
  uint8_t wartosc = cmdlineGetArgHex(2, state);

  sendSettings(adres, wartosc);

  return OK_SILENT;
}
static cliExRes_t zapiszModWykFunction(cmdState_t *state)
{
  if (state->argc < 1)
    return SYNTAX_ERROR;

  uint8_t adres =  cmdlineGetArgInt(1, state);
  saveSettings(adres);
  return OK_SILENT;
}

static cliExRes_t setMacAddrFunction(cmdState_t *state)
{
  if (state->argc < 6)
    return SYNTAX_ERROR;

  nicState.mac.addr[0] = cmdlineGetArgHex(1, state);
  nicState.mac.addr[1] = cmdlineGetArgHex(2, state);
  nicState.mac.addr[2] = cmdlineGetArgHex(3, state);
  nicState.mac.addr[3] = cmdlineGetArgHex(4, state);
  nicState.mac.addr[4] = cmdlineGetArgHex(5, state);
  nicState.mac.addr[5] = cmdlineGetArgHex(6, state);
  nicSetMacAddress(nicState.mac.addr);
  return OK_SILENT;
}

static cliExRes_t czytajAC_Function(cmdState_t *state)
{
  uint8_t nrWejscia = cmdlineGetArgInt(1, state);
  uint16_t wynik = MCP3008_getSampleSingle(nrWejscia);
  fprintf_P(state->myStdInOut, PSTR("Wartosc probki na wejsciu %d: %d\r\n"), nrWejscia, wynik);
  return OK_SILENT;
}

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}

static cliExRes_t pingFunction(cmdState_t *state)
{
  if (state->argc < 4)
    return SYNTAX_ERROR;

  //uint8_t ip[4];
  //ip[0] = (uint8_t)(cmdlineGetArgInt(1, state));
  //ip[1] = (uint8_t)(cmdlineGetArgInt(2, state));
  //ip[2] = (uint8_t)(cmdlineGetArgInt(3, state));
  //ip[3] = (uint8_t)(cmdlineGetArgInt(4, state));
  //Ipv4Ping(*((uint32_t *)(ip)));

  return OK_SILENT;
}

static cliExRes_t eraseRamFileFunction(cmdState_t *state)
{
  if (ramDyskUsunPlik(cmdlineGetArgStr(1, state)) == 0)
    return OK_INFORM;

  printErrorInfo(state);
  return ERROR_INFORM;
}

static cliExRes_t dodajRamPlikFunction(cmdState_t *state)
{
  if (state->argc != 1)
    return SYNTAX_ERROR;

  if (ramDyskUtworzPlik(cmdlineGetArgStr(1, state)) == 0)
  {
    return OK_INFORM;
  }
  printErrorInfo(state);
  return ERROR_INFORM;
}

static cliExRes_t writeRamFileFunction(cmdState_t *state)
{
  ramDyskDir(state->myStdInOut);
  return OK_SILENT;
}

static cliExRes_t editRamFileFunction(cmdState_t *state)
{
  if (ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty) != 0)
  {
    fprintf_P(state->myStdInOut, errorOpenFile, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
  }
  ramDyskUstawWskaznikNaKoniec(&fdVty);
  uint8_t znak = 0;
  fprintf_P(state->myStdInOut, editRamFileIntroStr);
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

static cliExRes_t readRamFIleFunction(cmdState_t *state) //TODO move this code to fat8
{
  uint8_t rezultat;
  uint8_t znak = ' ';
  if ((rezultat = ramDyskOtworzPlik(cmdlineGetArgStr(1, state), &fdVty)) != 0)
  {
    fprintf_P(state->myStdInOut, errorOpenFile, cmdlineGetArgStr(1, state));
    return ERROR_INFORM;
  }
  uint16_t rozmiar = fdVty.wpis->rozmiarHi * 256 + fdVty.wpis->rozmiarLo;
  fprintf_P(state->myStdInOut, readRamFIleLenStr , rozmiar);
  while (rezultat == 0)
  {
    rezultat = ramDyskCzytajBajtZPliku(&fdVty, &znak);

    uartVtySendByte(znak);
    if (znak == '\r')
      uartVtySendByte('\n');
  }
  fprintf_P(state->myStdInOut, nlStr);
  ramDyskZamknijPlik(&fdVty);
  return OK_SILENT;
}

static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  (void) state;
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


