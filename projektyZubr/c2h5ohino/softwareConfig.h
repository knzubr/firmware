#ifndef SOFTWARE_CONFIG_H
#define SOFTWARE_CONFIG_H 1

#include <inttypes.h>

/* Only one language can be available */
#define LANG_EN 1
#define LANG_PL 0


/* CLI */
#define CMD_STATE_HISTORY 4
#define CMD_STATE_HISTORY_MASK 0x03


#define HEAP_BEGIN                   0x0100
#define HEAP_END                     CLI_1_BUF_ADDR - 1
#define HEAP_SIZE                    HEAP_END - HEAP_BEGIN + 1

#define CLI_BUF_TOT_LEN              0x0100
#define CLI_1_BUF_ADDR               0x2800



/* Memory Map
 0x0000 +-----------------------------+
  256   | Controll registers          |
 0x0100 +-----------------------------+
  4k    | Internal memory             |










 0x1FFF +-----------------------------+
*/


#endif
