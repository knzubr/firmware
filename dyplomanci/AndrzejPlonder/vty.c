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

static cliExRes_t helpFunction           (cmdState_t *state);
static cliExRes_t statusFunction         (cmdState_t *state);
static cliExRes_t enableFunction         (cmdState_t *state);
static cliExRes_t disableFunction        (cmdState_t *state);
static cliExRes_t configureModeFunction  (cmdState_t *state);
static cliExRes_t saveConfigFunction     (cmdState_t *state);

static cliExRes_t sendPelcoMessage       (cmdState_t *state);
static cliExRes_t translateFunction      (cmdState_t *state);
static cliExRes_t showTableFunction      (cmdState_t *state);

const char okStr[] PROGMEM = "OK\r\n";
const char nlStr[] PROGMEM = "\r\n";

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
  {cmd_show_tt,   cmd_help_show_tt,   showTableFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_show_tt,   cmd_help_show_tt,   showTableFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_pelcoSnd,  cmd_help_pelcoSnd,  sendPelcoMessage},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_show_tt,   cmd_help_show_tt,   showTableFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_translate,    cmd_help_translate,    translateFunction},
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

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}

static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  (void) state;
  saveConfiguration();
  return OK_SILENT;
}

static cliExRes_t sendPelcoMessage(cmdState_t *state)
{
  if (state->argc < 5)
    return SYNTAX_ERROR;

  struct pelcoMsg tmpMsg;
  tmpMsg.addr   = (uint8_t)(cmdlineGetArgInt(1, state));
  tmpMsg.cmd[0] = (uint8_t)(cmdlineGetArgInt(2, state));
  tmpMsg.cmd[1] = (uint8_t)(cmdlineGetArgInt(3, state));
  tmpMsg.opt[0] = (uint8_t)(cmdlineGetArgInt(4, state));
  tmpMsg.opt[1] = (uint8_t)(cmdlineGetArgInt(5, state));


  xQueueSend(pelcoMessages, &tmpMsg, portMAX_DELAY);

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

static cliExRes_t translateFunction(cmdState_t *state)
{
  if (state->argc < 2)
    return SYNTAX_ERROR;

  uint8_t oldAddress = (uint8_t)(cmdlineGetArgInt(1, state));
  uint8_t newAddress = (uint8_t)(cmdlineGetArgInt(2, state));
  translateTable[oldAddress] = newAddress;
  return OK_SILENT;
}

static void printTable(FILE *stream)
{
  fprintf_P(stream, PSTR("\r\nAddressTable:\r\n\r\n"));  //Print system state

  uint16_t i;
  for(i=0; i<256; i++)
  {

      if (translateTable[i] != i)     /*???*/
       {

        if (i<101)
        {
            fprintf_P(stream, PSTR("%d -> %d\t\t"), i, translateTable[i]);
            i++ ;
            fprintf_P(stream, PSTR("%d -> %d\t\t"), i, translateTable[i]);
            i++ ;
            fprintf_P(stream, PSTR("%d -> %d\t\t"), i, translateTable[i]);
            i++ ;
            fprintf_P(stream, PSTR("%d -> %d\r\n"), i, translateTable[i]);
            /*i++ ;
            fprintf_P(stream, PSTR("%d -> %d\r\n"), i, translateTable[i]);

            fprintf_P(stream, PSTR("\r\n"));*/
        }
        else
        {
            fprintf_P(stream, PSTR("%d -> %d\t"), i, translateTable[i]);
            i++ ;
            fprintf_P(stream, PSTR("%d -> %d\t"), i, translateTable[i]);
            i++ ;
            fprintf_P(stream, PSTR("%d -> %d\t"), i, translateTable[i]);
            i++ ;
            fprintf_P(stream, PSTR("%d -> %d\r\n"), i, translateTable[i]);
            /* i++ ;
            fprintf_P(stream, PSTR("%d -> %d\r\n"), i, translateTable[i]);

            fprintf_P(stream, PSTR("\r\n"));  */
        }
       }
  }
  fprintf_P(stream, PSTR("\r\n"));
}

static cliExRes_t showTableFunction(cmdState_t *state)
{
  printTable(state->myStdInOut);
  return OK_SILENT;
}


