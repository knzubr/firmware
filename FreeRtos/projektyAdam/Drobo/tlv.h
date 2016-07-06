#ifndef TLV
#define TLV


#include "../../Lib/include/tlvProt.h"
#include "hardware.h"

typedef enum
{
  STOP         = 0,
  FORWARD      = 1,
  BACKWORD     = 2,
  ROTATE_LEFT  = 3,
  ROTATE_RIGHT = 4
} moveDirection_t;


//MSG type = 1
typedef struct
{
  tlvMsg_t         header;
  moveDirection_t  direction;
  uint8_t          duration;
  uint8_t          pwmLeft;
  uint8_t          pwmRight;
} tlvMsgMove_t;



#endif // TLV
