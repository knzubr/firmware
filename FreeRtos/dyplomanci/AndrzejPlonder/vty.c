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
static cliExRes_t rpingFunction          (cmdState_t *state);
//static cliExRes_t debugFunction          (cmdState_t *state);
static cliExRes_t enableFunction         (cmdState_t *state);
static cliExRes_t disableFunction        (cmdState_t *state);
static cliExRes_t configureModeFunction  (cmdState_t *state);
static cliExRes_t saveConfigFunction     (cmdState_t *state);


struct ramPlikFd    fdVty;  //TODO move it to CLI struct

const char okStr[] PROGMEM = "OK\r\n";
const char nlStr[] PROGMEM = "\r\n";
const char BladBuforaPozostaloBajtowStr[] PROGMEM = "!!! W budorze Rs485 pozostalo %d bajtow\r\n";


const const char* const errorStrings[] PROGMEM = {
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
  {cmd_enable,    cmd_help_enable,    enableFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_rping,     cmd_help_rping,     rpingFunction},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_enable,       cmd_help_enable,       enableFunction},
  {cmd_disable,      cmd_help_disable,      disableFunction},
  {NULL, NULL, NULL}
};

void VtyInit(cmdState_t* state, FILE *stream)
{
  cmdStateConfigure(state, (char *)(xmalloc(CLI_BUF_TOT_LEN)), CLI_BUF_TOT_LEN, stream, &cmdListNormal[0], NR_NORMAL);
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
}


// ************************** CLI Functions *********************************************************************************

static cliExRes_t statusFunction(cmdState_t *state)
{
  printStatus(state->myStdInOut);
  return OK_SILENT;
}

//static cliExRes_t debugFunction          (cmdState_t *state)
//{
//  if (state->argc < 2)
//    return SYNTAX_ERROR;

//  uint8_t level = cmdlineGetArgInt(2, state);
//  const char *str = (const char*)cmdlineGetArgStr(1, state);
//  if (level == 0)
//  {

//  }
//  else                   //level > 0
//  {

//  }

//  return SYNTAX_ERROR;
//}

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}


static cliExRes_t rpingFunction(cmdState_t *state)
{
  if (state->argc < 1)
    return SYNTAX_ERROR;

  uint8_t nrSterownika = (uint8_t)(cmdlineGetArgInt(1, state));
  if ((state->err2 = rs485ping(nrSterownika)) == 0)
    return OK_INFORM;

  state->errno = noRemoteDevice;
  state->err1 = nrSterownika;
  printErrorInfo(state);
  return OK_SILENT;
}

static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  (void) state;
  saveConfiguration();
  return OK_SILENT;
}



