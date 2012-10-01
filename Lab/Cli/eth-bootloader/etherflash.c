/*! \file "etherflash.c" \brief main bootloader routine  */
//***************************************************************************
//*            etherflash.c
//*
//*  Sun Feb  8 2009
//*  Copyright  Jan Krause
//*  Email issjagut [ at ] gmail.com
//****************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
//@{

#include <stdlib.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <avr/iom128.h>

#include "config.h"
#include "eemem.h"
#include "enc28j60.h"
#include "ethernet.h"
#include "arp.h"
#include "udp.h"
#include "dhcpc.h"
#include "etherflash.h"


uint8_t lineBuffer[78];
uint32_t baseAddress;
uint16_t bytesInBootPage;
uint32_t currentAddress;
struct UDP_SOCKET sock;
uint16_t tftpTimeoutCounter;
uint8_t hex_or_eep = 0xFF; // 0 =  flash , 1 =  eeprom, other =  other kind of file;
uint8_t lastPacket = 0;
//uint8_t bootload = 55;
// local functions
void initializeHardware (void) BOOTLOADER_SECTION;
void sendchar (char);
void putstring (char*);


//*******************************************************************************
//
// Initiate Watchdog
//
//*******************************************************************************

void watchdogInit(void)
{
 wdt_reset(); 
 wdt_enable(WDTO_2S);  
}

  

void initializeHardware (void)
{
	// reset hardware register
	// disable TWI
	TWCR &= ~(1<<TWIE);

	// disable INT2
#if defined GICR && defined INT2
	GICR &= ~(1<<INT2);
#elif defined EIMSK
	EIMSK = 0;
#endif

#ifdef PORTA
	DDRA = 0;
	PORTA = 0;
#endif
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	DDRE = 0;
	DDRF = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;
	PORTF = 0;
	
	// disable SPI
	SPCR &= ~(1<<SPE);
	
}

int main(void)
{
	// disable interrupts
	cli();
	//initialize wtachdog
	watchdogInit();
	//initializeHardware();
	initializeHardware();
	
#if DEBUG_AV
	// Debugging trough UART
	UCSR1B = ( 1 << TXEN );							// UART TX
	UCSR1C |= ( 1<<UCSZ10 )|( 1<<UCSZ11 );	  	        // Asynchron 8N1
	
#define BAUD 115200L
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
	UBRR1H = UBRR_VAL >> 8;
	UBRR1L = UBRR_VAL & 0xFF;
	
	putpgmstring("\r\n");
	putpgmstring("***********************\r\n");
	putpgmstring("Start\r\n");
#endif	
	
	
#define UART_INFO 1
#if UART_INFO
#define BAUD 115200L
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
	UBRR1H = UBRR_VAL >> 8;
	UBRR1L = UBRR_VAL & 0xFF;
	UCSR1B = ( 1 << TXEN );							// UART TX 
	UCSR1C |= ( 1<<UCSZ10 )|( 1<<UCSZ11 );	  	        // Asynchron 8N1
	UBRR1H = UBRR_VAL >> 8;
	UBRR1L = UBRR_VAL & 0xFF;
	putstring("BOOTLOADER START\r\n");
	
#endif		
	
	wdt_reset();
	wdt_disable(); 
	// initialize ENC28J60
	ETH_INIT();
	watchdogInit();
#if DEBUG_AV
	putpgmstring("ETH_INIT\r\n");
#endif
	
	ETH_PACKET_SEND(60,ethernetbuffer);
	ETH_PACKET_SEND(60,ethernetbuffer);

	// Clear receive buffer of ENC28J60
	while (ETH_PACKET_RECEIVE (MTU_SIZE, ethernetbuffer) != 0 ) {};

	stack_init ();
#if DEBUG_AV
	putpgmstring("stack_init done\r\n");
#endif	


// at this point we should have a valid IP-Address and be ready
// to execute our Application or to start the boot loader

// do something here to make a decision where to go
	
// for now, we're just jumping to the boot loader
   
        wdt_reset(); 
	BootLoaderMain();	// pBootloader();
	
// this call will never return. BootLoader will reset device when done
	return(0);
}


void BootLoaderMain(void)
{
	uint8_t nRetryCounter = 0;

	// init global vars
	baseAddress = 0;
	bytesInBootPage = 0;
	currentAddress = 0;
	tftpTimeoutCounter = 0;
	
	sock.DestinationIP = 0;
	sock.Bufferfill = 0;
	sock.BlockNumber = 0;
	sock.lineBufferIdx = 0;
	sock.SourcePort = TFTP_SERVER_PORT;
	
	// Register socket for WRQ
	UDP_RegisterSocket (sock.SourcePort, (void(*)(void))tftp_get);
	
	while (1)
	{
		wdt_reset(); 
		eth_packet_dispatcher();
	  	_delay_ms(1);
		
		if (tftpTimeoutCounter++ > TFTP_TIMEOUT)
		{
			tftpTimeoutCounter = 0;
		
#if DEBUG_AV && DEBUG_TFTP
			putpgmstring("TFTP timeout\r\n");
#endif	
			
			if ((sock.DestinationIP != 0) && (nRetryCounter++ < 10) && (hex_or_eep<2))
			{	// ok, we had contact from a server
				//putstring ("*");
				//send last ACK
				putstring("* Send block ACK again\r\n");
				UDP_SendPacket (4, sock.SourcePort, sock.DestinationPort, sock.DestinationIP);
			}
			else
			{
#if DEBUG_AV
		tftpTimeoutCounter = 0;	// stay in this loop, do nothing
#else
				if(nRetryCounter++ > 10)
				{
					putstring ("BOOTLOADER TIMEOUT\r\n");
					if(hex_or_eep<2)
					if(!lastPacket)
					 clearFlash();
   
					jumpToApplication();
				}
				else
					putstring ("#");
						  
			}
#endif	
		}
		
	}
	
}


void tftp_get (void)
{   

	struct ETH_header * ETH_packet; 		// ETH_struct anlegen
	ETH_packet = (struct ETH_header *) ethernetbuffer;
	struct IP_header * IP_packet;		// IP_struct anlegen
	IP_packet = ( struct IP_header *) &ethernetbuffer[ETH_HDR_LEN];
	struct UDP_header * UDP_packet;
	UDP_packet = ( struct UDP_header *) &ethernetbuffer[ETH_HDR_LEN + ((IP_packet->IP_Version_Headerlen & 0x0f) * 4 )];
	//UDP_packet = ( struct UDP_header *) &ethernetbuffer[UDP_OFFSET];
	struct TFTP_RESPONSE *tftp;
	tftp = (struct TFTP_RESPONSE *)&ethernetbuffer[ETH_HDR_LEN + ((IP_packet->IP_Version_Headerlen & 0x0f) * 4 ) + UDP_HDR_LEN];
	struct TFTP_REQUEST *tftpR;
	tftpR = (struct TFTP_REQUEST *)&ethernetbuffer[ETH_HDR_LEN + ((IP_packet->IP_Version_Headerlen & 0x0f) * 4 ) + UDP_HDR_LEN];
	
	

	
	
#if DEBUG_AV && DEBUG_TFTP
	putpgmstring("tftp_get()\r\n");
#endif	

	// Reset timeout counter
	tftpTimeoutCounter = 0;


	
	if (tftpR->op == 0x0200)
	{
		putstring ("WRQ");
		//need to check is it flash or eeprom image
		char * filename = &tftpR->filename;
		for(uint8_t i=0; i<15;i++)
		{
			if (*(filename+i) == 0x2e) // dot "." in name extension
			{
				filename=filename+i+1; 
				break;
			}
		}


			  putstring ("H"); 
		
		if (*(filename) =='e' && *(filename+1) =='e'  && *(filename+2) =='p' )
		{
			hex_or_eep = 0x01;
			putstring (" TO EEPROM\r\n");
		}
		else 
		  if (*(filename) =='h' && *(filename+1) =='e'  && *(filename+2) =='x' )
		{
			hex_or_eep = 0x00;
			putstring (" TO FLASH\r\n");
		}
		
		if(hex_or_eep<2)
		{
			sock.DestinationIP = IP_packet->IP_Srcaddr;
			UDP_UnRegisterSocket(sock.SourcePort)	;
			sock.SourcePort=5000;
			sock.DestinationPort=htons(UDP_packet->UDP_SourcePort);
			UDP_RegisterSocket (sock.SourcePort, (void(*)(void))tftp_get);
			arp_entry_add(IP_packet->IP_Srcaddr, ETH_packet->ETH_sourceMac); 
			tftp->op = 0x0400;
			// mark buffer free
			sock.Bufferfill = 0;
			tftp->blockNumber=0;
			//send ACK for WRQ
			UDP_SendPacket (4, sock.SourcePort, sock.DestinationPort, sock.DestinationIP);
		}
		return;
	}
	
if(hex_or_eep<2)
{		
	
	if (sock.DestinationIP != IP_packet->IP_Srcaddr)
	{	// other TFTP-Server is sending data - ignore it.
#if DEBUG_AV && DEBUG_TFTP
	putpgmstring("Ignoring packet from wrong server\r\n");
#endif	
		return;
	}

	// TFTP: Zielport ändern auf SourcePort des empfangenen Pakets (TID)
	sock.DestinationPort = htons(UDP_packet->UDP_SourcePort);

	if ((sock.BlockNumber + 1) != htons(tftp->blockNumber))
	{
		// this block is not expected
		if (htons(tftp->blockNumber) == (sock.BlockNumber))
		{
			// this block is the last block processed -> send ACK again (UDP is unreliable)
			tftp->op = 0x0400;
			// mark buffer free
			sock.Bufferfill = 0;
#if DEBUG_AV && DEBUG_TFTP
	puthexbyte(sock.BlockNumber>>8);
	puthexbyte(sock.BlockNumber);
	putpgmstring(" send block ACK again\r\n");
#endif		
			UDP_SendPacket (4, sock.SourcePort, sock.DestinationPort, sock.DestinationIP);
		}
#if DEBUG_AV && DEBUG_TFTP
		else
		{
			puthexbyte(tftp->blockNumber);
			puthexbyte(tftp->blockNumber>>8);
			putpgmstring("Ignoring packet with wrong block number\r\n");
		}
#endif	
		return;
	}

	// set current block number
	sock.BlockNumber = htons(tftp->blockNumber);


#if DEBUG_AV && DEBUG_TFTP
	puthexbyte(sock.BlockNumber>>8);
	puthexbyte(sock.BlockNumber);
	putpgmstring(" block processing\r\n");
#endif	

	// Größe der Daten eintragen
	sock.Bufferfill = htons(UDP_packet->UDP_Datalenght) - UDP_HDR_LEN;

	// last packet is shorter than 516 bytes
	if (sock.Bufferfill < 516)
		lastPacket = 1;

	// Offset für UDP-Daten im Ethernetfrane berechnen
	
	sock.DataStartOffset = ETH_HDR_LEN + ((IP_packet->IP_Version_Headerlen & 0x0f) * 4 ) + UDP_HDR_LEN;
	
	// check for data packet (00 03)
	//if (ethernetbuffer[sock.DataStartOffset+1] == 0x03)
	if (tftp->op == TFTP_OP_DATA)
	{	// this is a data packet
		uint16_t rxBufferIdx = sock.DataStartOffset+4;
		//uint16_t rxBufferIdx = 0;
		// copy current line till newline character or end of rx buf
		while ((rxBufferIdx - sock.DataStartOffset) < sock.Bufferfill)
		//while ((rxBufferIdx) < (sock.Bufferfill-4))
		{

			// copy next byte from rx buf to line buf
			//lineBuffer[sock.lineBufferIdx++] = tftp->data[rxBufferIdx++]; 
			lineBuffer[sock.lineBufferIdx++] = ethernetbuffer[rxBufferIdx++];
			if (ethernetbuffer[rxBufferIdx-1] == 0x0A)
			//if (tftp->data[rxBufferIdx-1] == 0x0A)
			{
				// newline
#if DEBUG_AV && #DEBUG_FLASH_PROG
	lineBuffer[sock.lineBufferIdx] = 0; // mark end of string
	putstring(lineBuffer);
#endif	
				processLineBuffer(sock.lineBufferIdx);
				sock.lineBufferIdx = 0;
			}
		}
		
		// rx buf processed
		// send ack and wait for next packet
		//uint8_t *udpSendBuffer = ethernetbuffer + (ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
		//uint8_t *udpSendBuffer = &ethernetbuffer[UDP_DATA_START];
		//udpSendBuffer[0]  = 0x00;
		//udpSendBuffer[1]  = 0x04; //TFTP_ACK
		tftp->op = 0x0400;

		// mark buffer free
		sock.Bufferfill = 0;
#if DEBUG_AV && DEBUG_TFTP
	puthexbyte(tftp->blockNumber);
	puthexbyte(tftp->blockNumber>>8);
	putpgmstring(" block ACK sending\r\n");
#endif			
		UDP_SendPacket (4, sock.SourcePort, sock.DestinationPort, sock.DestinationIP);
		
		if (lastPacket)
		{
			UDP_UnRegisterSocket(sock.SourcePort);
			// sometimes the hexfile doesn't end with a 0x01 Record (End Of File)
				// so we have to check if there is unwritten data in the buffer
			if (bytesInBootPage > 0)
			{
				writeFLASHPage(currentAddress);        
			}
#if DEBUG_AV
	#if DEBUG_TFTP
		putpgmstring("TFTP Done\r\n");
	#endif
#else
			putstring ("PROGRAMMING END\r\n");
			//putstring ("JUMP TO APPLICATION\r\n");
			if(!hex_or_eep)
			{
			  hex_or_eep=0xFF;
			  BootLoaderMain();
			}
			
			jumpToApplication();
#endif	
		}
	}
	//else if (ethernetbuffer[sock.DataStartOffset+1] == 5)
	else if (tftp->op == TFTP_OP_ERR)
	{
		// error -> reboot to application
#if DEBUG_AV
	#if DEBUG_TFTP
		putpgmstring("TFTP error\r\n");
		puthexbyte(tftp->errCode>>8);
		puthexbyte(tftp->errCode);
		putpgmstring("\r\n");
	#endif
#else
		clearFlash();
		UDP_UnRegisterSocket(sock.SourcePort);
		//if(!lastPacket)	reboot(); //for security

		jumpToApplication();
#endif	
	}
}
return;
}


uint8_t hexToByte(uint8_t *buf, uint16_t idx)
{
  uint8_t val = 0;
  uint8_t i, t;
  for (i=0; i<2; i++)
  {
    t = buf[idx+i];
    if (t > (uint8_t)'9')
    {
      // hex digit a-f
      val = (val<<4) + t - (uint8_t)'A' + 10;
    } else
    {
      // digit 1-9
      val = (val<<4) + t - (uint8_t)'0';
    }
  }    
  return val;
}

// interrupts have to be disabled when calling this function!
void writeFLASHPage(uint32_t currentAddress)
{
	#if DEBUG_AV
		#if DEBUG_FLASH_PROG
		putpgmstring("writeFLASHPage\r\n");
		#endif
	#else
	    eeprom_busy_wait ();

	    boot_page_erase (currentAddress-2);		// Clear flash page
	    boot_spm_busy_wait ();      			// Wait until the memory is erased.					
	
	    boot_page_write (currentAddress-2);     // Store buffer in flash page.
	    boot_spm_busy_wait();       			// Wait until the memory is written.
		boot_rww_enable(); 						// Parts of the bootloader code may
												// be in the RWW section, so make sure
												// we can access it. Thanks to
												// Dirk Armbrust for the hint.
	#endif

    bytesInBootPage = 0;
}

void FillFlashPage(uint32_t currentAddress, uint8_t loByte, uint8_t hiByte)
{	// All SPM instructions must be in the NRWW section
	boot_page_fill_safe(currentAddress, loByte + (hiByte << 8));
}

// parse a line of the intel hex file. 
// every line starts with a colon, followed by ASCII hex representation of up to 21 data bytes
void processLineBuffer(uint8_t bytes)
{
	// ignore colon byte and decode ASCII hex code to binary
	uint8_t i;
	for (i=0; i<((bytes-3)/2); i++)
	{
		lineBuffer[i] = hexToByte(lineBuffer, (i*2)+1);
	}
			
	uint8_t len;
	
	switch (lineBuffer[3])
	{
		case 0x00: // data record
			len = lineBuffer[0];
			
			currentAddress = baseAddress + (((uint16_t)lineBuffer[1]) << 8) + lineBuffer[2];
			
			// copy data to boot page
		 
			
			for (i=0; i<len; i+=1) 
			{	wdt_reset();
#if DEBUG_AV
#if DEBUG_FLASH_PROG
				putpgmstring("boot_page_fill_safe\r\n");
#endif
#else
				
				if(hex_or_eep==0)
				{	//eeprom writing	
					FillFlashPage (currentAddress, lineBuffer[i+4+0], lineBuffer[i+4+1]);
				   	

				    currentAddress += 2;
				    bytesInBootPage += 2;
				    if (bytesInBootPage == SPM_PAGESIZE)
				    {
					    // page is full -> write it				
					    writeFLASHPage(currentAddress);
				    }
				    i++;
				}
				else 
				  if (hex_or_eep==1)//eeprom writing
				{
					eeprom_busy_wait(); 
					eeprom_write_byte (currentAddress, lineBuffer[i+4]);
					currentAddress += 1; 
#endif
				}
			}
			
			break;
		      
		case 0x01: // last record
            // write (incomplete page) if there are bytes in
            if (bytesInBootPage > 0)
            {
                writeFLASHPage(currentAddress);
            }
			break;
			
		case 0x02: // extended segment address record
            // if bytes are in the page buffer, first write the buffer
            if (bytesInBootPage > 0)
            {
                writeFLASHPage(currentAddress);        
            }
			baseAddress = (uint32_t)((lineBuffer[4] << 8) + lineBuffer[5]) << 4;
			break;
			
		case 0x03: // start segment address record
		case 0x05: // start linear address record
			// ignore, we know where to go after the flash
		
			break;
			
		case 0x04: // extended linear address record
			// will never show up on smaller devices (ATmega32, ATmega644) since
			// flash address fits in 16 bits, but in case we have a device with
			// more than 64k flash we need to set the upper adress word here
			//it may  appear in image downlowaded from another atmega!
            // if bytes are in the page buffer, first write the buffer
            if (bytesInBootPage > 0)
            {
                writeFLASHPage(currentAddress);        
            } 
            
			baseAddress = (uint32_t)((lineBuffer[4] << 8) + lineBuffer[5]) << 16;
			break;
	}
	
	
}

void jumpToApplication(void)
{
    
	// Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
	boot_rww_enable_safe();
	wdt_reset();
	wdt_disable(); 

	initializeHardware();
	putstring ("BOOTLOADER END\r\nJUMP TO APPLICATION\r\n\r\n");
	
	pApplication();

}


// Debugging
////#if DEBUG_AV

void sendchar (char Zeichen)
{
	while (!(UCSR1A & (1<<UDRE1)));
	UDR1 = Zeichen;
}



void putstring (char *string)
{
	while (*string)
	{ 
		sendchar (*string);
		string++;
	}
}
#if DEBUG_AV
void puthexbyte(uint8_t bt)
{
	uint8_t btnibble = (bt >> 4) & 0x0f;
	if (btnibble > 9)
		btnibble += 'a'-10;
	else
		btnibble += '0';
	sendchar(btnibble);

	btnibble = bt & 0x0f;
	if (btnibble > 9)
		btnibble += 'a'-10;
	else
		btnibble += '0';
	sendchar(btnibble);
}
void putPGMstring (const char *string)
{
	unsigned char c = 0;
	do
	{
		c = pgm_read_byte(string);
		sendchar (c);
		string++;
	} while (c!='\n');
}

#endif

void clearFlash(void)
{
  for (uint32_t i=0; i<(479*SPM_PAGESIZE); i+=SPM_PAGESIZE)  // 480 stron * 256 B = 122880
  {
	//putstring ("^");
	eeprom_busy_wait ();
	boot_page_erase (i-2);		// Clear flash page
	boot_spm_busy_wait();      	// Wait until the memory is erased.
  }
  
  boot_rww_enable(); 

}

