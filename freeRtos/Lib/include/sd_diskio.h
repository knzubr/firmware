#ifndef _DISKIO_H
#define _DISKIO_H

#include "integer.h"

#define _READONLY	0	/* 1: Read-only mode */
#define _USE_IOCTL	1

//@{

/* Status of Disk Functions */
typedef BYTE  DSTATUS;

/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */



/* Results of Disk Functions */
typedef enum {
  RES_OK = 0, /** 0: Function succeeded */
  RES_ERROR,  /** 1: Disk error */
  RES_NOTRDY, /** 2: Not ready */
  RES_PARERR  /** 3: Invalid parameter */
} DRESULT;


/**
 * Initialize Disk Drive
 */
DSTATUS disk_initialize (void);

/**
 * Read Partial Sector  
 * @param  dest   - Pointer to the destination object
 * @param  sector - Sector number (LBA)
 * @param  sofs   - Offset in the sector
 * @param  count  - Number of byter to read (bytes or word ?)
 * @return 
 */
DRESULT disk_readp (BYTE *dest, DWORD sector, WORD sofs, WORD count);

/**
 * Write Partial Sector
 * @param  buff   - Pointer to the data to be written, NULL:Initiate/Finalize write operation
 * @param  sc     - Sector number (LBA) or Number of bytes to send
 */
DRESULT disk_writep (const BYTE* buff, DWORD sc);

#define STA_NOINIT  0x01              /** Drive not initialized */
#define STA_NODISK  0x02              /** No medium in the drive */


DRESULT disk_ioctl (BYTE, BYTE, void*);
void	disk_timerproc (void);


/* Command code for disk_ioctrl() */

/* Generic command */
#define CTRL_SYNC           0   /* Mandatory for write functions */
#define GET_SECTOR_COUNT    1   /* Mandatory for only f_mkfs() */
#define GET_SECTOR_SIZE     2
#define GET_BLOCK_SIZE      3   /* Mandatory for only f_mkfs() */
#define CTRL_POWER          4
#define CTRL_LOCK           5
#define CTRL_EJECT          6
/* MMC/SDC command */
#define MMC_GET_TYPE        10
#define MMC_GET_CSD         11
#define MMC_GET_CID         12
#define MMC_GET_OCR         13
#define MMC_GET_SDSTAT      14
/* ATA/CF command */
#define ATA_GET_REV         20
#define ATA_GET_MODEL       21
#define ATA_GET_SN          22




/* Card type flags (CardType) */
#define CT_MMC      0x01              /** MMC ver 3 */
#define CT_SD1      0x02              /** SD  ver 1 */
#define CT_SD2      0x04              /** SD  ver 2 */
#define CT_SDC      (CT_SD1|CT_SD2)   /** SD */
#define CT_BLOCK    0x08              /** Block addressing */

//@}
#endif

