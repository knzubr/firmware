/**
 * @file     nic.h 
 * @version  0.2
 * @brief    Network Interface Card (NIC) software definition. 
 * @ingroup  network
 * @author   Pascal Stang, Adam Kaliszan
 * @defgroup nic Network Interface Card (NIC) software definition (nic.h)
 * @code #include "net/nic.h" @endcode
 * @par Description
 *      This is the software interface standard for network interface hardware
 *      as used by AVRlib.  Drivers for network hardware must implement these
 *      functions to allow upper network layers to initialize the interface,
 *      and send and receive net traffic.
 * 
 * Editor Tabs  : 4
 * Target MCU   : Atmel AVR series
 * Created      : 22.08.2004
 * Revised      : 28.11.2010
 *
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
 */
//@{

#include "nic.h"

static uint8_t   mymac_eep[6] __attribute__((section (".eeprom"))) = {0x54,0x55,0x58,0x10,0x00,0x25};


/* Weak functions, that hast to be overriden in hardware specyfic driver implementation i.e. in enc28j60 */
void         nicMacInit(void)                                    { }
void         nicSend(uint16_t len)                               {len    = 0; }
unsigned int nicPoll(void)                                       { return 0; }


static void nicBufferInit(void)
{
  nicState.buf         = (uint8_t *)(ENC28J60BUF_ADDR);
  nicState.bufferSize  = ENC28J60BUF_SIZE;
  memset(nicState.buf, 0, nicState.bufferSize);
  
  eeprom_read_block(&nicState.mac.addr, mymac_eep, 6);
}

void saveNic(void)
{
  eeprom_update_block(&nicState.mac.addr, mymac_eep, 6);
}

void nicInit()
{
  nicBufferInit();
  nicMacInit();
}

void         nicGetMacAddress(uint8_t* macaddr)
{
  strncpy((void *)(macaddr), (void *)(nicState.mac.addr), 6);
}

void         nicSetMacAddress(uint8_t* macaddr)
{
  strncpy((void *)(nicState.mac.addr), (void *)(macaddr), 6);
  nicMacInit();
}

void         nicRegDump(FILE *stream)
{
  fprintf_P(stream, PSTR("NIC reg dump not implemented\r\n"));
}

//@}
