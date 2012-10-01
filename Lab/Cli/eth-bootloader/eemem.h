/***************************************************************************
 *            eemem.h
 *
 *  11.10.2007
 *  Copyright  2007  jkrause
 * 
 *  Diese Datei enthält alle Definitionen von EEMEM Variablen, die
 *  im EEPROM gespeichert werden
 ****************************************************************************/

#ifndef __EEMEM_H__
	
	#define __EEMEM_H__


extern unsigned long mlIpEEP;
extern unsigned long mlNetmaskEEP;
//extern unsigned long  mlGatewayEEP;
//extern unsigned long  mlDNSserverEEP;

#ifdef FIXED_TFTP_SRV
extern unsigned long  mlTFTPipEEP;
#endif

typedef struct 
{
	unsigned short opcode;
	unsigned char fnandmode[]; //filename and mode
} TFTPREQ;
extern TFTPREQ maTFTPReqStr;
#define TFTPReqStrSize 		17		// stupid compiler does not want to calculate 
									// sizeof(maTFTPReqStr), has to be done by hand 

typedef struct 
{
	unsigned short opcode;
	unsigned short errcode;
	unsigned char errstring[];
} TFTPERR;
//extern TFTPERR maTFTPErrStr EEMEM;
#define TFTPErrStrSize 		1		// stupid compiler does not want to calculate 
									// sizeof(maTFTPErrStr), has to be done by hand 


#if USE_ENC28J60	
extern unsigned char enc28j60_config[];
#define ENC28J60_CONFIG_OFFSET_MAC 33
#endif

#endif
