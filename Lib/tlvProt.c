#include "tlvProt.h"

void tlvIinitializeInterpreter(tlvInterpreter_t *tlvInt, FILE *ioStr, FILE *errStr, const tlvCommand_t *commands)
{
  memset(tlvInt, 0, sizeof(struct tlvInterpreter));
  tlvInt->ioStr  = ioStr;
  tlvInt->errStr = errStr;
  tlvInt->commands = commands;

  while(commands->type != 0)
  {
    tlvInt->noOfCmds++;
    commands++;
  }
}

void tlvCalculateCrc(tlvMsg_t *message)
{
  uint16_t crc;
  crc = _crc16_update(0, message->address);
  crc = _crc16_update(0, message->type);
  crc = _crc16_update(0, message->dtaLen);

  uint8_t i;
  for (i=0; i<message->dtaLen; i++)
    crc = _crc16_update(0, message->data[i]);

  message->crcLo = (uint8_t) crc;
  message->crcHi = (uint8_t) crc>>8;
}

uint8_t tlvCheckCrc(tlvMsg_t *message)
{
  uint16_t crc;
  crc = _crc16_update(0, message->address);
  crc = _crc16_update(0, message->type);
  crc = _crc16_update(0, message->dtaLen);

  uint8_t i;
  for (i=0; i<message->dtaLen; i++)
    crc = _crc16_update(0, message->data[i]);

  uint8_t crcLo = (uint8_t) crc;
  uint8_t crcHi = (uint8_t) crc>>8;

  if (message->crcLo != crcLo)
    return 0;

  if (message->crcHi != crcHi)
    return 0;

  return 1;
}

void tlvProcessDta(tlvInterpreter_t *tlvInt, uint8_t dta)
{
  uint8_t i, j;
  struct tlvMsg *myRecMsg = (struct tlvMsg *)tlvInt->buffer;

  if (tlvInt->bufIdx >= TLV_BUF_LEN)
    tlvInt->bufIdx = 0;

  if (tlvInt->bufIdx == 0 && dta != TLV_SYNC)
    return;

  tlvInt->buffer[tlvInt->bufIdx] = dta;
  tlvInt->bufIdx++;

  if (tlvInt->bufIdx < sizeof(struct tlvMsg))
    return;

  if (tlvInt->bufIdx < myRecMsg->dtaLen)
    return;

  if (tlvCheckCrc(myRecMsg) == 0)
  {
    fprintf_P(tlvInt->errStr, PSTR("# CRC mismatch\r\n"));
    for (i=1; i<tlvInt->bufIdx; i++)
    {
      if (tlvInt->buffer[i] == TLV_SYNC)
      {
        for (j=0; i+j < tlvInt->bufIdx; j++)
          tlvInt->buffer[j] = tlvInt->buffer[i+j];

        tlvInt->bufIdx -= i;
        return;
      }
    }
    tlvInt->bufIdx = 0;
    return;
  }

  tlvCommand_t tmp;                                                     // We need to create this object. We can't directly
  for (i=0; i<tlvInt->noOfCmds; i++)
  {
      memcpy_P(&tmp, &tlvInt->commands[i], sizeof(tlvCommand_t));
      if (myRecMsg->type == tmp.type)
      {
        tmp.fun(tlvInt, myRecMsg);
        break;
      }
  }
  if (i == tlvInt->noOfCmds)
    fprintf_P(tlvInt->errStr, PSTR("! Unknown command: %d\r\n"), myRecMsg->type);
  tlvInt->bufIdx = 0;
}

void sendTlvMsg(tlvMsg_t *message, FILE *ostream)
{
  int i, len;
  len = sizeof(struct tlvMsg) + message->dtaLen;
  uint8_t *ptr = (uint8_t *) message;

  for (i=0; i<len; i++)
  {
    fputc(*ptr, ostream);
    ptr++;
  }
}
