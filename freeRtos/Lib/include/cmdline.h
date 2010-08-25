#ifndef CMDLINE_H
#define CMDLINE_H

//enum substitute
#define CMDLINE_HISTORY_SAVE  0
#define CMDLINE_HISTORY_PREV  1
#define CMDLINE_HISTORY_NEXT  2

//Configuration
#define CMDLINE_MAX_COMMANDS    20
#define CMDLINE_MAX_CMD_LENGTH  10
#define CMDLINE_BUFFERSIZE      80
#define CMDLINE_HISTORYSIZE     1


#include <avr/io.h>         // editor recognizes now types like uint8_t 

// constants/macros/typdefs
struct cmdState;
struct cmdList;

typedef struct cmdList  cmdList_t;
typedef struct cmdState cmdState_t;
typedef void (*CmdlineFuncPtrType)(cmdState_t *state);

struct cmdState
{
  char    *CmdlineBuffer;
  uint8_t bufferSize;
  uint8_t CmdlineBufferLength;
  uint8_t CmdlineBufferEditPos;
  
  uint8_t CmdlineInputVT100State;
  char    CmdlineHistory[CMDLINE_HISTORYSIZE][CMDLINE_BUFFERSIZE];
  CmdlineFuncPtrType CmdlineExecFunction;
  
  FILE myStdInOut;
};

struct cmdList
{ /// command list, each command is null-terminated string.
  char CmdlineCommandList[CMDLINE_MAX_COMMANDS][CMDLINE_MAX_CMD_LENGTH];
  
  /// function table
  CmdlineFuncPtrType CmdlineFunctionList[CMDLINE_MAX_COMMANDS];

  /// number of commands currently registered
  uint8_t CmdlineNumCommands;
};


// functions

//! initalize the command line system
void cmdlineInit(void);

//! add a new command to the database of known commands
// newCmdString should be a null-terminated command string with no whitespace
// newCmdFuncPtr should be a pointer to the function to execute when the user enters the corresponding command string
//   TODO remove this function and add constans table with strings and function pointers
void cmdlineAddCommand(char* newCmdString, CmdlineFuncPtrType newCmdFuncPtr);

//! call this function to pass input charaters from the user terminal
void cmdlineInputFunc(char c, cmdState_t *state);

//! call this function in your program's main loop
void cmdlineMainLoop(cmdState_t *state);

// internal commands
void cmdlineRepaint(cmdState_t *state);
void cmdlineDoHistory(char action, cmdState_t *state);
void cmdlineProcessInputString(cmdState_t *state);
void cmdlinePrintPrompt(cmdState_t *state);
void cmdlinePrintError(cmdState_t *state);

// argument retrieval commands
//! returns a string pointer to argument number [argnum] on the command line
char* cmdlineGetArgStr(uint8_t argnum, cmdState_t *state);

//! returns the decimal integer interpretation of argument number [argnum]
long cmdlineGetArgInt (uint8_t argnum, cmdState_t *state);

//! returns the hex integer interpretation of argument number [argnum]
long cmdlineGetArgHex (uint8_t argnum, cmdState_t *state);

void        cmdStateClear(cmdState_t *state);
cmdState_t* cmdStateCreate(uint8_t bufferSize, int (*output_func)(char c, FILE *stream));

#endif
