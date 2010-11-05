/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "enc_task.h"
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
//#include "timeout.h"
//#include "avr_compat.h"
#include "net.h"
#include "main.h"

// the password string (only the first 5 char checked), (only a-z,0-9,_ characters):
char password[]="secret"; // must not be longer than 9 char

uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};  // how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX
uint8_t myip[4] = {10,0,0,24};

uint8_t verify_password ( char *str )
{
    // the first characters of the received string are
    // a simple password/cookie:
    if ( strncmp ( password,str,5 ) ==0 )
    {
        return ( 1 );
    }
    return ( 0 );
}

int8_t analyse_get_url ( char *str )
{
    uint8_t loop=1;
    uint8_t i=0;
    while ( loop )
    {
        if ( password[i] )
        {
            if ( *str==password[i] )
            {
                str++;
                i++;
            }
            else
            {
                return ( -1 );
            }
        }
        else
        {
            // end of password
            loop=0;
        }
    }
    // is is now one char after the password
    if ( *str == '/' )
    {
        str++;
    }
    else
    {
        return ( -3 );
    }
    // check the first char, garbage after this is ignored (including a slash)
    if ( *str < 0x3a && *str > 0x2f )
    {
        // is a ASCII number, return it
        return ( *str-0x30 );
    }
    return ( -2 );
}

uint16_t moved_perm ( uint8_t *buf1 )
{
    uint16_t plen;
    plen=fill_tcp_data_p ( buf1, 0,    PSTR ( "HTTP/1.0 301 Moved Permanently\r\nLocation: " ) );
    plen=fill_tcp_data ( buf1, plen, password );
    plen=fill_tcp_data_p ( buf1, plen, PSTR ( "/\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n" ) );
    plen=fill_tcp_data_p ( buf1, plen, PSTR ( "<h1>301 Moved Permanently</h1>\n" ) );
    plen=fill_tcp_data_p ( buf1, plen, PSTR ( "add a trailing slash to the url\n" ) );
    return ( plen );
}

uint16_t print_webpage ( uint8_t *buf1,uint8_t on_off )
{
    uint16_t plen;
    plen=fill_tcp_data_p ( buf1, 0,    PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n" ) );
    plen=fill_tcp_data_p ( buf1, plen, PSTR ( "<center><p>Output is: " ) );
    if ( on_off )
    {
        plen=fill_tcp_data_p ( buf1, plen, PSTR ( "<font color=\"#00FF00\"> ON</font>" ) );
    }
    else
    {
        plen=fill_tcp_data_p ( buf1, plen,PSTR ( "OFF" ) );
    }
    plen=fill_tcp_data_p ( buf1, plen, PSTR ( " <small><a href=\".\">[refresh status]</a></small></p>\n<p><a href=\"." ) );
    if ( on_off )
    {
        plen=fill_tcp_data_p ( buf1, plen, PSTR ( "/0\">Switch off</a><p>" ) );
    }
    else
    {
        plen=fill_tcp_data_p ( buf1, plen, PSTR ( "/1\">Switch on</a><p>" ) );
    }
    plen=fill_tcp_data_p ( buf1, plen, PSTR ( "</center><hr><br>version 2.17, tuxgraphics.org\n" ) );
    return ( plen );
}

void enc_init ( void )
{
    buf = malloc(BUFFER_SIZE);
    vTaskDelay          (5);
    enc28j60Init        (mymac);
//  enc28j60clkout      (2);     // change clkout from 6.25MHz to 12.5MHz
    vTaskDelay          (5);
    enc28j60PhyWrite    (PHLCON, 0x476);
    vTaskDelay          (2);
    init_ip_arp_udp_tcp (mymac, myip, MYWWWPORT);

}


void encTask ( void *pvParameters )
{
    cmdState_t *state = (cmdState_t *)(pvParameters);
    uint16_t plen;
    uint16_t dat_p;
//  uint8_t i=1;
    uint8_t cmd_pos=0;
    int8_t cmd;
    uint8_t payloadlen=0;
    char str[30];
    char cmdval;

    enc_init();

    for ( ; ; )
    {
        vTaskDelay ( 0 );         //Zastąpić oczekiwaniem na zwolnienie semafora. Semafor zostaje zwolniony po odebrzeniu przerwania od ENC

        // get the next new packet:
        plen = enc28j60PacketReceive ( BUFFER_SIZE, buf );

        /*plen will ne unequal to zero if there is a valid
        * packet (without crc error) */
        if ( plen==0 )
            continue;
//        fprintf(&state->myStdInOut, "Dat %d\r\n", plen);

        // arp is broadcast if unknown but a host may also
        // verify the mac address by sending it to
        // a unicast address.
        if ( eth_type_is_arp_and_my_ip ( buf,plen ) )
        {
            make_arp_answer_from_request ( buf );
            continue;
        }

        // check if ip packets are for us:
        if ( eth_type_is_ip_and_my_ip ( buf,plen ) ==0 )
            continue;


        if ( buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V )
        {
            // a ping packet, let's send pong
            make_echo_reply_from_request ( buf,plen );
            continue;
        }
//        continue;

        // tcp port www start, compare only the lower byte
        if ( buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==MYWWWPORT )
        {
            if ( buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V )
            {
                make_tcp_synack_from_syn ( buf );
                // make_tcp_synack_from_syn does already send the syn,ack
                continue;
            }
            if ( buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V )
            {
                init_len_info ( buf ); // init some data structures
                // we can possibly have no data, just ack:
                dat_p=get_tcp_data_pointer();
                if ( dat_p==0 )
                {
                    if ( buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V )
                    {
                        // finack, answer with ack
                        make_tcp_ack_from_any ( buf );
                    }
                    // just an ack with no data, wait for next packet
                    continue;
                }
                if ( strncmp ( "GET ", ( char * ) & ( buf[dat_p] ),4 ) !=0 )
                {
                    // head, post and other methods:
                    //
                    // for possible status codes see:
                    // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                    plen=fill_tcp_data_p ( buf,0,PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>" ) );
                    goto SENDTCP;
                }
                if ( strncmp ( "/ ", ( char * ) & ( buf[dat_p+4] ),2 ) ==0 )
                {
                    plen=fill_tcp_data_p ( buf,0,PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
                    plen=fill_tcp_data_p ( buf,plen,PSTR ( "<p>Usage: http://host_or_ip/password</p>\n" ) );
                    goto SENDTCP;
                }
                cmd=analyse_get_url ( ( char * ) & ( buf[dat_p+5] ) );
                // for possible status codes see:
                // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                if ( cmd==-1 )
                {
                    plen=fill_tcp_data_p ( buf,0,PSTR ( "HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>" ) );
                    goto SENDTCP;
                }
//                 if ( cmd==1 )
//                 {
//                     PORTD|= ( 1<<PORTD7 );// transistor on
//                 }
//                 if ( cmd==0 )
//                 {
//                     PORTD &= ~ ( 1<<PORTD7 );// transistor off
//                 }
                if ( cmd==-3 )
                {
                    // redirect to add a trailing slash
                    plen=moved_perm ( buf );
                    goto SENDTCP;
                }
                // if (cmd==-2) or any other value
                // just display the status:
                plen=print_webpage ( buf, ( PORTD & ( 1<<PORTD7 ) ) );
                //
            SENDTCP:
                make_tcp_ack_from_any ( buf ); // send ack for http get
                make_tcp_ack_with_data ( buf,plen ); // send data
                continue;
            }
        }

        // tcp port www end
        //
        // udp start, we listen on udp port 1200=0x4B0
        if ( buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0 )
        {
            payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
            // you must sent a string starting with v
            // e.g udpcom version 10.0.0.24
            if ( verify_password ( ( char * ) & ( buf[UDP_DATA_P] ) ) )
            {
                // find the first comma which indicates
                // the start of a command:
                cmd_pos=0;
                while ( cmd_pos<payloadlen )
                {
                    cmd_pos++;
                    if ( buf[UDP_DATA_P+cmd_pos]==',' )
                    {
                        cmd_pos++; // put on start of cmd
                        break;
                    }
                }

                // a command is one char and a value. At
                // least 3 characters long. It has an '=' on
                // position 2:
                if ( cmd_pos<2 || cmd_pos>payloadlen-3 || buf[UDP_DATA_P+cmd_pos+1]!='=' )
                {
                    strcpy ( str,"e=no_cmd" );
                    goto ANSWER;
                }

                // supported commands are
                // t=1 t=0 t=?
                if ( buf[UDP_DATA_P+cmd_pos]=='t' )
                {
                    cmdval=buf[UDP_DATA_P+cmd_pos+2];
                    if ( cmdval=='1' )
                    {
                        PORTD|= ( 1<<PORTD7 );// transistor on
                        strcpy ( str,"t=1" );
                        goto ANSWER;
                    }
                    else if ( cmdval=='0' )
                    {
                        PORTD &= ~ ( 1<<PORTD7 );// transistor off
                        strcpy ( str,"t=0" );
                        goto ANSWER;
                    }
                    else if ( cmdval=='?' )
                    {
                        if ( PORTD & ( 1<<PORTD7 ) )
                        {
                            strcpy ( str,"t=1" );
                            goto ANSWER;
                        }
                        strcpy ( str,"t=0" );
                        goto ANSWER;
                    }
                }
                strcpy ( str,"e=no_such_cmd" );
                goto ANSWER;
            }
            strcpy ( str,"e=invalid_pw" );
        ANSWER:
            make_udp_reply_from_request ( buf,str,strlen ( str ),MYUDPPORT );
        }
    }
}