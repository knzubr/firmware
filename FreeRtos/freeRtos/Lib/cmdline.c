/*! \file cmdline.c \brief Command-Line Interface Library. */
//*****************************************************************************
//
// File Name    : 'cmdline.c'
// Title        : Command-Line Interface Library
// Author       : Pascal Stang - Copyright (C) 2003
//              : Adam Kaliszan
// Created      : 2003.07.16
// Revised      : 2010.04.23
// Version      : 0.2
// Target MCU   : Atmel AVR Series
// Editor Tabs  : 2
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

//----- Include Files ---------------------------------------------------------

#include "main.h"

#include <stdio.h>          // fprint() support
#include <avr/io.h>         // include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>  // include interrupt support
#include <avr/pgmspace.h>   // include AVR program memory support
#include <string.h>         // include standard C string functions
#include <stdlib.h>         // include stdlib for string conversion functions

#include "cmdline.h"        // Configuration
#include "vt100.h"          // vty100 constans




//Globals
cmdList_t  CLCommands;

// Constans Strings
char PROGMEM CmdlinePrompt[]      = "DomOs>";
char PROGMEM CmdlineNotice[]      = "cmdline: ";
char PROGMEM CmdlineCmdNotFound[] = "# nk";

void cmdStateConfigure(cmdState_t * state, char *buffPtr, uint16_t bufferTotalSize, int (*output_func)(char c, FILE *stream))
{
  memset(state, 0, sizeof(cmdState_t));
  memset(buffPtr, 0, bufferTotalSize);
    
  state->CmdlineBuffer = buffPtr;
  state->bufferMaxSize    = (uint8_t)(bufferTotalSize / CMD_STATE_HISTORY);

  uint8_t i;
  char *tmpPtr = buffPtr;
  for (i=0; i < CMD_STATE_HISTORY; i++)
  {
    state->CmdlineHistory[i] = tmpPtr;
    tmpPtr += state->bufferMaxSize;
  }
  
  //cmdStateClear(newCmdState);
  fdev_setup_stream(&state->myStdInOut, output_func, NULL, _FDEV_SETUP_WRITE);
  fdev_set_udata(&state->myStdInOut, state);
}

void cmdStateClear(cmdState_t *state)
{
  // reset vt100 processing state
  state->CmdlineInputVT100State = 0;

  // initialize input buffer
  state->CmdlineBufferLength = 0;
  state->CmdlineBufferEditPos = 0;

  // initialize executing function
  state->CmdlineExecFunction = 0;
}

void cmdlineInit(void)
{
  // initialize command list
  CLCommands.CmdlineNumCommands = 0;
}

void cmdlineAddCommand(char* newCmdString, CmdlineFuncPtrType newCmdFuncPtr)
{
  // add command string to end of command list
  strcpy(CLCommands.CmdlineCommandList[CLCommands.CmdlineNumCommands], newCmdString);
  // add command function ptr to end of function list
  CLCommands.CmdlineFunctionList[CLCommands.CmdlineNumCommands] = newCmdFuncPtr;
  // increment number of registered commands
  CLCommands.CmdlineNumCommands++;
}

void cmdlineInputFunc(char c, cmdState_t *state)
{
  uint8_t i;
  // process the received character

  // VT100 handling
  // are we processing a VT100 command?
  if(state->CmdlineInputVT100State == 2)
  {
    // we have already received ESC and [
    // now process the vt100 codeCmdlineExcBuffer
    switch(c)
    {
    case VT100_ARROWUP:
      cmdlineDoHistory(CMDLINE_HISTORY_PREV, state);
      break;
    case VT100_ARROWDOWN:
      cmdlineDoHistory(CMDLINE_HISTORY_NEXT, state);
      break;
    case VT100_ARROWRIGHT:
      // if the edit position less than current string length
      if(state->CmdlineBufferEditPos < state->CmdlineBufferLength)
      {
        // increment the edit position
        state->CmdlineBufferEditPos++;
        // move cursor forward one space (no erase)
        fputc(ASCII_ESC        , &state->myStdInOut);
        fputc('['              , &state->myStdInOut);
        fputc(VT100_ARROWRIGHT , &state->myStdInOut);
      }
      else
      {
        // else, ring the bell
        fputc(ASCII_BEL        , &state->myStdInOut);
      }
      break;
    case VT100_ARROWLEFT:
      // if the edit position is non-zero
      if(state->CmdlineBufferEditPos)
      {
        // decrement the edit position
        state->CmdlineBufferEditPos--;
        // move cursor back one space (no erase)
        fputc(ASCII_BS         , &state->myStdInOut);
      }
      else
      {
        // else, ring the bell
        fputc(ASCII_BEL        , &state->myStdInOut);
      }
      break;
    default:
      break;
    }
    // done, reset state
    state->CmdlineInputVT100State = 0;
    return;
  }
  else if(state->CmdlineInputVT100State == 1)
  {
    // we last received [ESC]
    if(c == '[')
    {
      state->CmdlineInputVT100State = 2;
      return;
    }
    else
      state->CmdlineInputVT100State = 0;
  }
  else
  {
    // anything else, reset state
    state->CmdlineInputVT100State = 0;
  }

  // Regular handling
  //Protection against buffer Overflow
  if (state->CmdlineBufferLength >= state->bufferMaxSize)
  {
    state->CmdlineBufferLength--;
    for (i=1; i < state->bufferMaxSize; i++)
    {
      state->CmdlineBuffer[i-1] = state->CmdlineBuffer[i];
    }
  }
  
  if( (c >= 0x20) && (c < 0x7F) )
  {
    // character is printable
    // is this a simple append
    if(state->CmdlineBufferEditPos == state->CmdlineBufferLength)
    {
      // echo character to the output
      fputc(c                , &state->myStdInOut);
      // add it to the command line buffer
      state->CmdlineBuffer[state->CmdlineBufferEditPos++] = c;
      // update buffer length
      state->CmdlineBufferLength++;
    }
    else
    {
      // edit/cursor position != end of buffer
      // we're inserting characters at a mid-line edit position
      // make room at the insert point
      state->CmdlineBufferLength++;
      for(i=state->CmdlineBufferLength; i>state->CmdlineBufferEditPos; i--)
        state->CmdlineBuffer[i] = state->CmdlineBuffer[i-1];
      // insert character
      state->CmdlineBuffer[state->CmdlineBufferEditPos++] = c;
      // repaint
      cmdlineRepaint(state, state->CmdlineBuffer);
      // reposition cursor
      for(i=state->CmdlineBufferEditPos; i<state->CmdlineBufferLength; i++)
        fputc(ASCII_BS         , &state->myStdInOut);
    }
  }
  // handle special characters
  else if(c == ASCII_CR)
  {
    // user pressed [ENTER]
    // echo CR and LF to terminal
    fputc(ASCII_CR         , &state->myStdInOut);
    fputc(ASCII_LF         , &state->myStdInOut);
    // add null termination to command
    state->CmdlineBuffer[state->CmdlineBufferLength++] = 0;
    state->CmdlineBufferEditPos++;
    // command is complete, process it
    cmdlineProcessInputString(state);
    // reset buffer
    state->CmdlineBufferLength = 0;
    state->CmdlineBufferEditPos = 0;
  }
  else if(c == ASCII_BS)
  {
    if(state->CmdlineBufferEditPos)
    {
      // is this a simple delete (off the end of the line)
      if(state->CmdlineBufferEditPos == state->CmdlineBufferLength)
      {
        // destructive backspace
        // echo backspace-space-backspace
        fputc(ASCII_BS         , &state->myStdInOut);
        fputc(' '              , &state->myStdInOut);
        fputc(ASCII_BS         , &state->myStdInOut);
        // decrement our buffer length and edit position
        state->CmdlineBufferLength--;
        state->CmdlineBufferEditPos--;
      }
      else
      {
        // edit/cursor position != end of buffer
        // we're deleting characters at a mid-line edit position
        // shift characters down, effectively deleting
        state->CmdlineBufferLength--;
        state->CmdlineBufferEditPos--;
        for(i=state->CmdlineBufferEditPos; i<state->CmdlineBufferLength; i++)
          state->CmdlineBuffer[i] = state->CmdlineBuffer[i+1];
        // repaint
        cmdlineRepaint(state, state->CmdlineBuffer);
        // add space to clear leftover characters
        fputc(' '              , &state->myStdInOut);
        // reposition cursor
        for(i=state->CmdlineBufferEditPos; i<(state->CmdlineBufferLength+1); i++)
          fputc(ASCII_BS       , &state->myStdInOut);
      }
    }
    else
    {
      // else, ring the bell
      fputc(ASCII_BEL          , &state->myStdInOut);
    }
  }
  else if(c == ASCII_DEL)
  {
    // not yet handled
  }
  else if(c == ASCII_ESC)
  {
    state->CmdlineInputVT100State = 1;
  }
}

void cmdlineRepaint(cmdState_t *state, char *buf)
{
  uint8_t i;

  // carriage return
  fputc(ASCII_CR         , &state->myStdInOut);
  // print fresh prompt
  cmdlinePrintPrompt(state);
  // print the new command line buffer
  i = state->CmdlineBufferLength;
  while(i--) 
    fputc(*buf++         , &state->myStdInOut);
}

void cmdlineDoHistory(char action, cmdState_t *state)
{
  switch(action)
  {
  case CMDLINE_HISTORY_SAVE:
    // copy CmdlineBuffer to history if not null string
    if( strlen(state->CmdlineBuffer) )
    {
      state->historyRdIdx = state->historyWrIdx;
      state->historyWrIdx++;
      if (state->historyWrIdx == CMD_STATE_HISTORY)
      {
        state->historyWrIdx = 0;
      }
      
      state->CmdlineBuffer = state->CmdlineHistory[state->historyWrIdx];
      memset(state->CmdlineBuffer, 0, state->bufferMaxSize);
      state->CmdlineBufferLength  = 0;
      state->CmdlineBufferEditPos = 0;
    }
    else
    {
      memset(state->CmdlineBuffer, 0, state->bufferMaxSize);
      state->CmdlineBufferLength  = 0;
      state->CmdlineBufferEditPos = 0;
    }
    break;
  case CMDLINE_HISTORY_PREV:
    if (state->historyRdIdx == state->historyWrIdx)
      break;

    if (state->CmdlineHistory[state->historyRdIdx][0] == 0)
      break;
    
    state->CmdlineBuffer = state->CmdlineHistory[state->historyRdIdx];

    // set the buffer position to the end of the line
    state->CmdlineBufferLength = strlen(state->CmdlineBuffer);
    state->CmdlineBufferEditPos = state->CmdlineBufferLength;

    // "re-paint" line
    cmdlineRepaint(state, state->CmdlineBuffer);

    // copy history to current buffer
    if (state->historyRdIdx == 0)
      state->historyRdIdx = CMD_STATE_HISTORY;
    state->historyRdIdx --;
    
    break;
  case CMDLINE_HISTORY_NEXT:      
    state->historyRdIdx++;
    if (state->historyRdIdx == CMD_STATE_HISTORY)
      state->historyRdIdx = 0;
    
    if (state->CmdlineHistory[state->historyRdIdx][0] != 0)
    {
      state->CmdlineBuffer = state->CmdlineHistory[state->historyRdIdx];

      // set the buffer position to the end of the line
      state->CmdlineBufferLength = strlen(state->CmdlineBuffer);
      state->CmdlineBufferEditPos = state->CmdlineBufferLength;

      // "re-paint" line
      cmdlineRepaint(state, state->CmdlineBuffer);
    }
    break;
  }
}

void cmdlineProcessInputString(cmdState_t *state)
{
  uint8_t cmdIndex;
  uint8_t i=0;

  state->CmdlineExcBuffer = state->CmdlineBuffer;

  // save command in history
  cmdlineDoHistory(CMDLINE_HISTORY_SAVE, state);

  // find the end of the command (excluding arguments)
  // find first whitespace character in CmdlineBuffer
  while( !((state->CmdlineExcBuffer[i] == ' ') || (state->CmdlineExcBuffer[i] == 0)) ) i++;

  if(!i)
  {
    // command was null or empty
    // output a new prompt
    cmdlinePrintPrompt(state);
    // we're done
    return;
  }

  // search command list for match with entered command
  for(cmdIndex=0; cmdIndex< CLCommands.CmdlineNumCommands; cmdIndex++)
  {
    if( !strncmp(CLCommands.CmdlineCommandList[cmdIndex], state->CmdlineExcBuffer, i) )
    {
      // user-entered command matched a command in the list (database)
      // run the corresponding function
      state->CmdlineExecFunction = CLCommands.CmdlineFunctionList[cmdIndex];
      // new prompt will be output after user function runs
      // and we're done
      return;
    }
  }

  // if we did not get a match
  // output an error message
  cmdlinePrintError(state);
  // output a new prompt
  cmdlinePrintPrompt(state);
}

void cmdlineMainLoop(cmdState_t *state)
{
  // do we have a command/function to be executed
  if(state->CmdlineExecFunction)
  {
    // run it
    state->CmdlineExecFunction(state);
    // reset
    state->CmdlineExecFunction = 0;
    // output new prompt
    cmdlinePrintPrompt(state);
  }
}

void cmdlinePrintPrompt(cmdState_t *state)
{
  // print a new command prompt
  char* ptr = CmdlinePrompt;
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , &state->myStdInOut);
}

void cmdlinePrintError(cmdState_t *state)
{
  char * ptr;

  // print a notice header
  // (uint8_t*) cast used to avoid compiler warning
  ptr = (char*)CmdlineNotice;
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , &state->myStdInOut);
  
  // print the offending command
  ptr = state->CmdlineBuffer;
  while((*ptr) && (*ptr != ' '))
    fputc(*ptr++    , &state->myStdInOut);

  fputc(':'         , &state->myStdInOut);
  fputc(' '         , &state->myStdInOut);

  // print the not-found message
  // (uint8_t*) cast used to avoid compiler warning
  ptr = (char*)CmdlineCmdNotFound;
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , &state->myStdInOut);

  fputc('\r'        , &state->myStdInOut);
  fputc('\n'        , &state->myStdInOut);
}

// argument retrieval commands

// return string pointer to argument [argnum]
char* cmdlineGetArgStr(uint8_t argnum, cmdState_t *state)
{
  // find the offset of argument number [argnum]
  uint8_t idx=0;
  uint8_t arg;
  
  // find the first non-whitespace character
  while( (state->CmdlineExcBuffer[idx] != 0) && (state->CmdlineExcBuffer[idx] == ' ')) idx++;
  
  // we are at the first argument
  for(arg=0; arg<argnum; arg++)
  {
    // find the next whitespace character
    while( (state->CmdlineExcBuffer[idx] != 0) && (state->CmdlineExcBuffer[idx] != ' ')) idx++;
    // find the first non-whitespace character
    while( (state->CmdlineExcBuffer[idx] != 0) && (state->CmdlineExcBuffer[idx] == ' ')) idx++;
  }
  // we are at the requested argument or the end of the buffer
  return &state->CmdlineExcBuffer[idx];
}

// return argument [argnum] interpreted as a decimal integer
long cmdlineGetArgInt(uint8_t argnum, cmdState_t *state)
{
  char* endptr;
  return strtol(cmdlineGetArgStr(argnum, state), &endptr, 10);
}

// return argument [argnum] interpreted as a hex integer
long cmdlineGetArgHex(uint8_t argnum, cmdState_t *state)
{
  char* endptr;
  return strtol(cmdlineGetArgStr(argnum, state), &endptr, 16);
}
