#ifndef CMDLINE_H
#define CMDLINE_H

//enum substitute
#define CMDLINE_HISTORY_SAVE  0
#define CMDLINE_HISTORY_PREV  1
#define CMDLINE_HISTORY_NEXT  2


#include <avr/io.h>         // editor recognizes now types like uint8_t 
#include <avr/pgmspace.h>

// constants/macros/typdefs
struct cmdState;
struct command;

typedef struct command  command_t;
typedef struct cmdState cmdState_t;

typedef void (*CmdlineFuncPtrType)(cmdState_t *state);

enum cmdBufferHistory
{
  NOT_COPIED   = 0,
  COPIED       = 1
};

enum cliModeState
{
  NR_NORMAL,
  NR_ENABLE,
  NR_CONFIGURE,
  RESTRICTED_NORMAL
};

#define CMD_STATE_HISTORY 4
#define CMD_STATE_HISTORY_MASK 0x03

struct cmdState
{
  char *CmdlineBuffer;                       /// CLI buffer.
  char *CmdlineExcBuffer;                    /// CLI processing buffer.
  char *CmdlineHistory[CMD_STATE_HISTORY];   /// CLI history. History Size = 3. Sorry for Hardcodding

  uint8_t bufferMaxSize;                     /// Total buffer size / CMD_STATE_HISTORY
  uint8_t CmdlineBufferLength;
  uint8_t CmdlineBufferEditPos;
 
  uint8_t historyWrIdx;                      /// History write index (0 - CMD_STATE_HISTORY-1)   
  uint8_t historyDepthIdx;                   /// History depth index. Read idx = (historyWrIdx - historyDepthIdx) & CMD_STATE_HISTORY_MASK
  enum cmdBufferHistory bufferHistoryState;
    
  uint8_t CmdlineInputVT100State;
  CmdlineFuncPtrType CmdlineExecFunction;
  
  FILE myStdInOut;
  
  uint8_t errno;                             /// Error number
  uint8_t err1;                              /// Additional error info 1
  uint8_t err2;                              /// Additional error info 1
  
  enum cliModeState cliMode;
  const command_t *cmdList;
};

struct command
{
  prog_char           *commandStr;
  prog_char           *commandHelpStr;
  CmdlineFuncPtrType  commandFun;
};


// functions

//! call this function to pass input charaters from the user terminal
void cmdlineInputFunc(char c, cmdState_t *state);

//! call this function in your program's main loop
void cmdlineMainLoop(cmdState_t *state);

// internal commands
void cmdlineRepaint            (cmdState_t *state, char *buf);
void cmdlineDoHistory          (char action, cmdState_t *state);
void cmdlineProcessInputString (cmdState_t *state);
void cmdlinePrintPrompt        (cmdState_t *state);
void cmdlinePrintError         (cmdState_t *state);

// argument retrieval commands
//! returns a string pointer to argument number [argnum] on the command line
char* cmdlineGetArgStr(uint8_t argnum, cmdState_t *state);

//! returns the decimal integer interpretation of argument number [argnum]
long cmdlineGetArgInt (uint8_t argnum, cmdState_t *state);

//! returns the hex integer interpretation of argument number [argnum]
long cmdlineGetArgHex (uint8_t argnum, cmdState_t *state);

void cmdStateClear(cmdState_t *state);

void cmdHistoryCopy(cmdState_t *state);

void cmdHistoryMove(cmdState_t *state);

/**
 * Print all commands available for cmdState and its description
 * @param state - command line interpreter state
 */
void cmdPrintHelp(cmdState_t *state);

/**
 * Konfiguruje strukturę do obsługi sesji interpretera poleceń
 * @param state            - wskaźnik do struktury ze stanem sesji interpretera poleceń
 * @param buffPtr          - wskaźnik do początku bufora. 1/4 bufora przeznaczona jest na buforowanie polecenia, 3/4 na zapamiętanie 3 ostatnich poleceń
 * @param bufferTotalSize  - długość przydzielonego bufora. Min 32 * CMD_STATE_HISTORY bajtów
 * @param output_func      - wskaźnik do funkcji obsługującej strumień wyjściowy
 */
void cmdStateConfigure(cmdState_t * state, char *buffPtr, uint16_t bufferTotalSize, int (*output_func)(char c, FILE *stream), const command_t *commands, enum cliModeState mode);

#endif