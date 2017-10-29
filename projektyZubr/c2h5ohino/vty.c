#include "main.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "softwareConfig.h"

#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif


static cliExRes_t helpFunction               (cmdState_t *state);
static cliExRes_t statusFunction             (cmdState_t *state);
static cliExRes_t saveConfigFunction         (cmdState_t *state);

static cliExRes_t disableFunction            (cmdState_t *state);
static cliExRes_t enableFunction             (cmdState_t *state);
static cliExRes_t configureModeFunction      (cmdState_t *state);
static cliExRes_t checkFunction              (cmdState_t *state);

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
  {cmd_enable,    cmd_help_enable,    enableFunction},
  {cmd_check,     cmd_help_check,     checkFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {cmd_check,     cmd_help_check,     checkFunction},
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
  cmdStateConfigure(state, cliBuffer, CLI_BUF_TOT_LEN, stream, &cmdListNormal[0], NR_NORMAL);
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

// ************************** CLI Functions *********************************************************************************

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}

static cliExRes_t statusFunction(cmdState_t *state)
{
  fprintf_P(state->myStdInOut, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));
  //Print system state
  fprintf_P(state->myStdInOut, systemStateStr);
  fprintf_P(state->myStdInOut, statusNumberOfTasksStr,    uxTaskGetNumberOfTasks());
  fprintf_P(state->myStdInOut, statusStaticHeapStateStr,  xPortGetFreeHeapSize(), configTOTAL_HEAP_SIZE);
  return OK_SILENT;
}

static cliExRes_t checkFunction(cmdState_t *state)
{
  uint8_t chanNo = 0;
  if (state->argc > 1)
    chanNo =cmdlineGetArgInt(1, state);

  adcSignle(chanNo);
  fprintf_P(state->myStdInOut, PSTR("Rozpoczynanie pomiaru na kanale %d"), chanNo);

  uint16_t wynik = adcRead();

  fprintf_P(state->myStdInOut, PSTR(" -> %d"), wynik);
  return OK_SILENT;
}
static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  (void) state;
  saveConfiguration();
  return OK_SILENT;
}
