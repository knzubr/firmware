#ifndef CMDLINE_H
#define CMDLINE_H

//enum substitute
#define CMDLINE_HISTORY_SAVE  0
#define CMDLINE_HISTORY_PREV  1
#define CMDLINE_HISTORY_NEXT  2

//Configuration
#define CMDLINE_MAX_COMMANDS    20
#define CMDLINE_MAX_CMD_LENGTH  10


#include <avr/io.h>         // editor recognizes now types like uint8_t 

// constants/macros/typdefs
struct cmdState;
struct cmdList;

typedef struct cmdList  cmdList_t;
typedef struct cmdState cmdState_t;
typedef void (*CmdlineFuncPtrType)(cmdState_t *state);


enum bufferHistory
{
  EMPTY,
  NOT_COPIED,
  COPIED
};

#define CMD_STATE_HISTORY 4
struct cmdState
{
  char *CmdlineBuffer;                       /// CLI buffer.
  char *CmdlineExcBuffer;                    /// CLI processing buffer.
  char *CmdlineHistory[CMD_STATE_HISTORY];   /// CLI history. History Size = 3. Sorry for Hardcodding

  uint8_t bufferMaxSize;                     /// Total buffer size / CMD_STATE_HISTORY
  uint8_t CmdlineBufferLength;
  uint8_t CmdlineBufferEditPos;
 
  uint8_t historyRdIdx;                      /// History read index  (0 - CMD_STATE_HISTORY-1)
  uint8_t historyWrIdx;                      /// History write index (0 - CMD_STATE_HISTORY-1) 
  
  uint8_t historyOffset;  
  enum bufferHistory bufferHistoryState;
    
  uint8_t CmdlineInputVT100State;
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

/**
 * Konfiguruje strukturę do obsługi sesji interpretera poleceń
 * @param state            - wskaźnik do struktury ze stanem sesji interpretera poleceń
 * @param buffPtr          - wskaźnik do początku bufora. 1/4 bufora przeznaczona jest na buforowanie polecenia, 3/4 na zapamiętanie 3 ostatnich poleceń
 * @param bufferTotalSize  - długość przydzielonego bufora. Min 32 * CMD_STATE_HISTORY bajtów
 * @param output_func      - wskaźnik do funkcji obsługującej strumień wyjściowy
 */
void cmdStateConfigure(cmdState_t * state, char *buffPtr, uint16_t bufferTotalSize, int (*output_func)(char c, FILE *stream));

#endif
