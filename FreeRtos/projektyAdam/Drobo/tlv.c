#include "tlv.h"

static void forwardFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void backwordFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void rotateLeftFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void rotateRightFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void stopFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);

const tlvCommand_t tlvCmdList[] PROGMEM =
{
  {FORWARD,       forwardFunction},
  {BACKWORD,      backwordFunction},
  {ROTATE_LEFT,   rotateLeftFunction},
  {ROTATE_RIGHT,  rotateRightFunction},
  {STOP,          stopFunction},
  {0,             NULL}
};

static void forwardFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  forwardB(msg->pwmLeft, msg->pwmRight);
}

static void backwordFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  backwordB(msg->pwmLeft, msg->pwmRight);
}

static void rotateLeftFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  rotateLeftB(msg->pwmLeft, msg->pwmRight);
}

static void rotateRightFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  rotateRightB(msg->pwmLeft, msg->pwmRight);
}

static void stopFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)

{
  (void) tlvInt;
  (void) myTlvMsg;
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  offHbridge();
}
