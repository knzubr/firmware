#include "tlv.h"

static void forwardFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void backwordFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void rotateLeftFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void rotateRightFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void stopFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void pingFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);

const tlvCommand_t tlvCmdList[] PROGMEM =
{
  {FORWARD,       forwardFunction},
  {BACKWORD,      backwordFunction},
  {ROTATE_LEFT,   rotateLeftFunction},
  {ROTATE_RIGHT,  rotateRightFunction},
  {STOP,          stopFunction},
  {PING,          pingFunction},
  {0,             NULL}
};

static void forwardFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  forwardB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void backwordFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  backwordB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void rotateLeftFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  rotateLeftB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void rotateRightFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  rotateRightB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void stopFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  offHbridge();
}

static void pingFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  sendTlvMsgDta(myTlvMsg, myTlvMsg->data, tlvInt->errStr);
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
}
