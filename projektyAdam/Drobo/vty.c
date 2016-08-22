#include "main.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "softwareConfig.h"
#include "hardware.h"

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

static cliExRes_t forwardFunction        (cmdState_t *state);
static cliExRes_t backwordFunction       (cmdState_t *state);
static cliExRes_t rotateLeftFunction     (cmdState_t *state);
static cliExRes_t rotateRightFunction    (cmdState_t *state);
static cliExRes_t stopFunction           (cmdState_t *state);

static cliExRes_t hc12modeFunction       (cmdState_t *state);
static cliExRes_t hc12channelFunction    (cmdState_t *state);
static cliExRes_t hc12baudrateFunction   (cmdState_t *state);
static cliExRes_t hc12powerFunction      (cmdState_t *state);

static cliExRes_t hc12sendForwardFunction    (cmdState_t *state);
static cliExRes_t hc12sendBackwordFunction   (cmdState_t *state);
static cliExRes_t hc12sendRotateLeftFunction (cmdState_t *state);
static cliExRes_t hc12sendRotateRightFunction(cmdState_t *state);
static cliExRes_t hc12sendStopFunction       (cmdState_t *state);


static cliExRes_t sendHC12(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t cmdDta[]);
static cliExRes_t sendHC12AtCmd(cmdState_t *state, const char cmd[] );

static cliExRes_t sendHC12loopback(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t cmdDta[]);


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
  {cmd_help,            cmd_help_help,            helpFunction},
  {cmd_status,          cmd_help_status,          statusFunction},
  {cmd_enable,          cmd_help_enable,          enableFunction},
  {cmd_forward,         cmd_help_forward,         forwardFunction},
  {cmd_backward,        cmd_help_backward,        backwordFunction},
  {cmd_rotateLeft,      cmd_help_rotateLeft,      rotateLeftFunction},
  {cmd_rotateRight,     cmd_help_rotateRight,     rotateRightFunction},
  {cmd_stop,            cmd_help_stop,            stopFunction},
  {cmd_hc12forward,     cmd_help_hc12forward,     hc12sendForwardFunction},
  {cmd_hc12backward,    cmd_help_hc12backward,    hc12sendBackwordFunction},
  {cmd_hc12rotateLeft,  cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction},
  {cmd_hc12rotateRight, cmd_help_hc12rotateRight, hc12sendRotateRightFunction},
  {cmd_hc12stop,        cmd_help_hc12stop,        hc12sendStopFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,        cmd_help_help,        helpFunction},
  {cmd_status,      cmd_help_status,      statusFunction},
  {cmd_disable,     cmd_help_disable,     disableFunction},
  {cmd_configure,   cmd_help_configure,   configureModeFunction},
  {cmd_forward,     cmd_help_forward,     forwardFunction},
  {cmd_backward,    cmd_help_backward,    backwordFunction},
  {cmd_rotateLeft,  cmd_help_rotateLeft,  rotateLeftFunction},
  {cmd_rotateRight, cmd_help_rotateRight, rotateRightFunction},
  {cmd_stop,        cmd_help_stop,        stopFunction},
  {cmd_hc12forward,     cmd_help_hc12forward,     hc12sendForwardFunction},
  {cmd_hc12backward,    cmd_help_hc12backward,    hc12sendBackwordFunction},
  {cmd_hc12rotateLeft,  cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction},
  {cmd_hc12rotateRight, cmd_help_hc12rotateRight, hc12sendRotateRightFunction},
  {cmd_hc12stop,        cmd_help_hc12stop,        hc12sendStopFunction},
  {NULL, NULL, NULL}
};

const command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_HC12mode,     cmd_help_HC12mode,     hc12modeFunction},
  {cmd_HC12channel,  cmd_help_HC12channel,  hc12channelFunction},
  {cmd_HC12baudrate, cmd_help_HC12baudrate, hc12baudrateFunction},
  {cmd_HC12power,    cmd_help_HC12power,    hc12powerFunction},

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


static cliExRes_t forwardFunction        (cmdState_t *state)
{
    /*
  uint8_t left = 50;
  uint8_t right = 50;

  if (state->argc == 1)
    left = right = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    left = cmdlineGetArgInt(1, state);
    right = cmdlineGetArgInt(2, state);
  }

  forwardB(left, right);
*/


  uint8_t dta[3];
  dta[0]=0;
  dta[1] = 50;
  dta[2] = 50;

  if (state->argc == 1)
    dta[1] = dta[2] = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta[1] = cmdlineGetArgInt(1, state);
    dta[2] = cmdlineGetArgInt(2, state);
  }

  sendHC12loopback(state, 0, FORWARD, 3, dta);
  return OK_SILENT;
}

static cliExRes_t backwordFunction       (cmdState_t *state)
{
  uint8_t left = 50;
  uint8_t right = 50;

  if (state->argc == 1)
    left = right = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    left = cmdlineGetArgInt(1, state);
    right = cmdlineGetArgInt(2, state);
  }

  backwordB(left, right);

  return OK_SILENT;
}
static cliExRes_t rotateLeftFunction     (cmdState_t *state)
{
  uint8_t left = 50;
  uint8_t right = 50;

  if (state->argc == 1)
    left = right = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    left = cmdlineGetArgInt(1, state);
    right = cmdlineGetArgInt(2, state);
  }

  rotateLeftB(left, right);

  return OK_SILENT;
}

static cliExRes_t rotateRightFunction    (cmdState_t *state)
{
  uint8_t left = 50;
  uint8_t right = 50;

  if (state->argc == 1)
    left = right = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    right = cmdlineGetArgInt(1, state);
    left = cmdlineGetArgInt(2, state);
  }

  rotateRightB(left, right);

  return OK_SILENT;
}

static cliExRes_t stopFunction           (cmdState_t *state)
{
  (void) state;
  offHbridge();
  return OK_SILENT;
}

static cliExRes_t hc12sendForwardFunction    (cmdState_t *state)
{
  uint8_t dta[3];
  dta[0]=0;
  dta[1] = 50;
  dta[2] = 50;

  if (state->argc == 1)
    dta[1] = dta[2] = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta[1] = cmdlineGetArgInt(1, state);
    dta[2] = cmdlineGetArgInt(2, state);
  }

  sendHC12(state, 0, FORWARD, 3, dta);
  return OK_SILENT;
}

static cliExRes_t hc12sendBackwordFunction   (cmdState_t *state)
{
  uint8_t dta[3];
  dta[0]=0;
  dta[1] = 50;
  dta[2] = 50;

  if (state->argc == 1)
    dta[1] = dta[2] = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta[1] = cmdlineGetArgInt(1, state);
    dta[2] = cmdlineGetArgInt(2, state);
  }

  sendHC12(state, 0, BACKWORD, 3, dta);
  return OK_SILENT;
}


static cliExRes_t hc12sendRotateLeftFunction    (cmdState_t *state)
{
  uint8_t msgDta[3];

  msgDta[0] = 0;     //duration
  msgDta[1] = 50;    //PWM left
  msgDta[2] = 50;    //PWM right

  if (state->argc == 1)
    msgDta[1] = msgDta[2] = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    msgDta[1] = cmdlineGetArgInt(1, state);
    msgDta[2] = cmdlineGetArgInt(2, state);
  }

  sendHC12(state, 0, ROTATE_LEFT, 3, msgDta);

  return OK_SILENT;
}

static cliExRes_t hc12sendRotateRightFunction    (cmdState_t *state)
{
  uint8_t msgDta[3];

  msgDta[0] = 0;     //duration
  msgDta[1] = 50;    //PWM left
  msgDta[2] = 50;    //PWM right

  if (state->argc == 1)
    msgDta[1] = msgDta[2] = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    msgDta[1] = cmdlineGetArgInt(2, state);
    msgDta[2] = cmdlineGetArgInt(1, state);
  }

  sendHC12(state, 0, ROTATE_RIGHT, 3, msgDta);

  return OK_SILENT;
}

static cliExRes_t hc12sendStopFunction       (cmdState_t *state)
{
  sendHC12(state, 0, STOP, 0, NULL);
  return OK_SILENT;
}

static cliExRes_t sendHC12AtCmd(cmdState_t *state, const char cmd[])
{
  if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
  {
    vTaskDelay(2);
    HC12setAtMode();

    fprintf_P(state->myStdInOut, cmd, cmdlineGetArgStr(1, state));
    fprintf_P(&hc12Stream,       cmd, cmdlineGetArgStr(1, state));

    uint8_t val;
    while (xQueueReceive(xHC12Rec, &val, 100) == pdTRUE)
    {
        fputc(val, state->myStdInOut);
    }

    HC12setTransparentMode();
    xSemaphoreGive(Hc12semaphore );
    return OK_INFORM;
  }
  else
  {
    return ERROR_INFORM;
  }

}

static cliExRes_t sendHC12(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t cmdDta[])
{
  tlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12Stream);

  if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
  {
    vTaskDelay(2);
    HC12setTransparentMode();


    //fprintf_P(state->myStdInOut, cmd, cmdlineGetArgStr(1, state));
    fprintf_P(&hc12Stream, (const char *) cmdDta, cmdlineGetArgStr(1, state));

    uint8_t val;
    while (xQueueReceive(xHC12Rec, &val, 100) == pdTRUE)
    {
        fputc(val, state->myStdInOut);
    }

    xSemaphoreGive(Hc12semaphore );
    return OK_INFORM;
  }
  else
  {
    return ERROR_INFORM;
  }

}

static cliExRes_t sendHC12loopback(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t cmdDta[])
{
  tlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12FakeStream);

  if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
  {
    vTaskDelay(2);
    HC12setTransparentMode();


    //fprintf_P(state->myStdInOut, cmd, cmdlineGetArgStr(1, state));
    //fprintf_P(&hc12Stream, (const char *) cmdDta, cmdlineGetArgStr(1, state));

    uint8_t val;
    while (xQueueReceive(xHC12Rec, &val, 100) == pdTRUE)
    {
        fputc(val, state->myStdInOut);
    }

    xSemaphoreGive(Hc12semaphore );
    return OK_INFORM;
  }
  else
  {
    return ERROR_INFORM;
  }

}

static cliExRes_t hc12modeFunction       (cmdState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+FU%s\r\n"));
}

static cliExRes_t hc12channelFunction    (cmdState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+C%s\r\n"));
}

static cliExRes_t hc12baudrateFunction   (cmdState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+B%s\r\n"));
}

static cliExRes_t hc12powerFunction      (cmdState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+P%s\r\n"));
}


/*
static void printTable(FILE *stream)
{
  fprintf_P(stream, PSTR("\r\nAddress Table:\r\n\r\n"));  //Print system state

  uint16_t i;
  for(i=0; i<256; i++)
  {
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\r\n"), i, translateTable[i]);
  }
  fprintf_P(stream, PSTR("\r\n"));
}
*/
