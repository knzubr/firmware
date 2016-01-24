#include "Rs485_prot.h"
#include <util/crc16.h>
#include <stdio.h>

#if LANG_EN
#include "Rs485_prot_en.h"
#define PRINT_RS485_DEVICE 1
#endif

#if LANG_PL
#include "Rs485_prot_pl.h"
#define PRINT_RS485_DEVICE 1
#endif
