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
#include "netstack_task.h"

// the password string (only the first 5 char checked), (only a-z,0-9,_ characters):
//char password[]="secret"; // must not be longer than 9 char
//char filename[12]="xxx";

// uint8_t verify_password ( char *str )
// {
//     // the first characters of the received string are
//     // a simple password/cookie:
//     if ( strncmp ( password,str,5 ) ==0 )
//     {
//         return ( 1 );
//     }
//     return ( 0 );
// }


// urlSource_t analyse_get_url (const char *str, char *fname)
// {
//   uint8_t result = URLerror;
//   if (strncmp_P(str, PSTR("status"), 6) == 0)
//     return URLstatus;
//     
//   if (strncmp_P(str, PSTR("rd/"), 3) == 0)
//   {
//     result = URLramDysk;
//     str += 3;
//   }
// 
//   if (strncmp_P(str, PSTR("sd/"), 3) == 0)
//   {
//     result = URLsdDysk;
//     str += 3;
//   }
//    
//   uint8_t i=0;
//   while (i < 12)
//   {
//     if (((*str >= 'a') && (*str <= 'z')) || ((*str >= 'A') && (*str <= 'Z')) || ((*str >= '0') && (*str <= '9')))
//     {
//       fname[i] = *str;
//     }
//     else
//     {
//       fname[i] = 0;
//       break;
//     }
//     str++;
//     i++;
//   }
//   if (i == 0)
//     result = URLerror;
//   
//   return result;
// }

/*uint16_t printHTMLstatus(char *buf, uint16_t pos, uint16_t maxPos)
{*/
/*  char *tmpPtr;

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ( "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ( "<h3>Status</h3>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<p>"SYSTEM_NAME" ver <b>"S_VERSION"</b> build: "__DATE__", "__TIME__"</p>"));
  
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<p><table border=1>"));

  tmpPtr = getBufPosToWrite(buf, pos);
  pos +=sprintf_P(tmpPtr, PSTR("<tr><td>Temperatura</td><td>%d C</td></tr>"), temperature);
  tmpPtr = getBufPosToWrite(buf, pos);
  pos +=sprintf_P(tmpPtr, PSTR("<tr><td>Napięcie na magistrali</td><td>%d V</td></tr>"), voltage);

  uint8_t tmp = ramDyskLiczbaWolnychKlastrow();
  tmpPtr = getBufPosToWrite(buf, pos);
  pos +=sprintf_P(tmpPtr, PSTR("<tr><td>Liczba wolnych klastrów</td><td>%d / %d</td></tr>"), tmp, L_KLASTROW);

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("</table></p>"));

  tmpPtr = getBufPosToWrite(buf, pos);
  
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR("<h3>Czujniki rygli</h3>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<p><table border=1>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<tr><td>Czujnik nr</td><td>Położenie rygla</td><td>Odczyt z przetwornika AC</td><td>Wart graniczna</td></tr>"));
  for (tmp = 0; tmp < 4; tmp++)
  {
    if (lockSensors[tmp].enabled)
    {
      tmpPtr = getBufPosToWrite(buf, pos);
      pos +=sprintf_P(tmpPtr, PSTR("<tr align=\"center\"><td>%d</td>"), tmp+1);
      if (lockSensors[tmp].locked)
        pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<td>zamknięty</td>"));
      else
        pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<td>otwarty</td>"));

      tmpPtr = getBufPosToWrite(buf, pos);
      pos +=sprintf_P(tmpPtr, PSTR("<td>%d</td><td>%d</td>"), lockSensors[tmp].acVal, lockSensors[tmp].threshold);
      pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("</tr>"));
    }
  }
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("</table></p>"));

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR("<h3>Moduły wykonawcze</h3>"));
  return pos;*/
//   return 0;
// }


void encTask ( void *pvParameters )
{
  FILE *netstackDebug = (FILE *) pvParameters;
  uint16_t plen;

  nicInit();
  ipInit();
  arpInit();
  icmpInit();

  //TODO    init_ip_arp_udp_tcp (mymac, ipGetConfig()->ip, MYWWWPORT);

  struct netEthHeader* ethPacket;
  
  
  for ( ; ; )
  {
    vTaskDelay ( 0 );         //Zastąpić oczekiwaniem na zwolnienie semafora. Semafor zostaje zwolniony po odebrzeniu przerwania od ENC
    
    // get the next new packet:
    plen = nicPoll();
    /*plen will ne unequal to zero if there is a valid
    * packet (without crc error) */
    if ( plen==0 )
      continue;

    ethPacket = (struct netEthHeader *)(nicState.buf);
    
    if(ethPacket->type == htons(ETHTYPE_IP))             // process an IP packet
    {
      arpIpIn((struct netEthIpHeader*)(nicState.buf));
      netstackIPProcess(plen-ETH_HEADER_LEN, (ip_hdr*)(&nicState.buf[ETH_HEADER_LEN]));
    }
    else if(ethPacket->type == htons(ETHTYPE_ARP))       // process an ARP packet
    {
      arpArpIn(plen, ((struct netEthArpHeader*)(nicState.buf)));
    }
    else
    {
      if (netstackDebug != NULL)
      {
        fprintf_P(netstackDebug, PSTR("Unknown packet\r\n"));
      }
    }
    
    continue;
  }
}

#if 0
//   uint16_t dat_p;
//   uint8_t cmd_pos=0;
//   int8_t cmd;
//   uint8_t payloadlen=0;
//   char str[30];
//   char cmdval;


    // arp is broadcast if unknown but a host may also
    // verify the mac address by sending it to
    // a unicast address.  
    if ( eth_type_is_arp_and_my_ip(Enc28j60_global.buf, plen))
    {
      make_arp_answer_from_request(Enc28j60_global.buf);
      continue;
    }

    // IP - check if ip packets are for us:
    if ( eth_type_is_ip_and_my_ip ( Enc28j60_global.buf,plen ) ==0 )
      continue;

    // PING
    if ( Enc28j60_global.buf[IP_PROTO_P]==IP_PROTO_ICMP && Enc28j60_global.buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V )
    {
      // a ping packet, let's send pong
      make_echo_reply_from_request (Enc28j60_global.buf, plen);
      continue;
    }

    // TCP WWW tcp port www start, compare only the lower byte
    if ( Enc28j60_global.buf[IP_PROTO_P]==IP_PROTO_TCP && Enc28j60_global.buf[TCP_DST_PORT_H_P]==0 && Enc28j60_global.buf[TCP_DST_PORT_L_P]==MYWWWPORT )
    {
      if ( Enc28j60_global.buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V )
      {
        make_tcp_synack_from_syn (Enc28j60_global.buf );
        // make_tcp_synack_from_syn does already send the syn,ack
        continue;
      }
      if (Enc28j60_global.buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
      {
        init_len_info (Enc28j60_global.buf ); // init some data structures
        // we can possibly have no data, just ack:
        dat_p=get_tcp_data_pointer();
        if ( dat_p==0 )
        {
          if (Enc28j60_global.buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V )
          {
            // finack, answer with ack
            make_tcp_ack_from_any (Enc28j60_global.buf );
          }
          // just an ack with no data, wait for next packet
          continue;
        }
        if ( strncmp ( "GET ", ( char * )(&Enc28j60_global.buf[dat_p]), 4) !=0 )
        {
          // head, post and other methods:
          //
          // for possible status codes see:
          // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
          plen=fill_tcp_data_p (Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>" ) );
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;
        }
        if ( strncmp ( "/ ", ( char * ) & (Enc28j60_global.buf[dat_p+4] ),2 ) ==0)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Usage: http://host_or_ip/filename or http://host_or_ip/sd/filename</p>\n" ) );
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;
        }

        cmd = analyse_get_url (( char * )(&Enc28j60_global.buf[dat_p+5]), filename);

        if (cmd == URLramDysk)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
  
          //Open the filen
          struct ramPlikFd fd;
          if (ramDyskOtworzPlik(filename, &fd) != 0)
          {
            plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Nie mozna otworzyc pliku o nazwie: " ) );
            plen=fill_tcp_data(Enc28j60_global.buf, plen, filename);
            plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( " umieszczonego w ram dysku</p>\n" ) );
            make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
            make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
            continue;
          }
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Zawartosc pliku " ) );
          plen=fill_tcp_data(Enc28j60_global.buf, plen, filename);
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( " zapisanego w ram dysku:<br>" ) );

          char c;
          while (plen < Enc28j60_global.bufferSize - 30)
          {
            if (ramDyskCzytajBajtZPliku(&fd , &c) != 0)
              break;
            plen = add_tcp_byte(Enc28j60_global.buf, plen, c);
          }
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "</p>\n"));
          ramDyskZamknijPlik(&fd);
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data

          continue;
        }

        if (cmd == URLsdDysk)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Do zaimpelentowania wyswietlenie pliku o nazwie " ) );
          plen=fill_tcp_data(Enc28j60_global.buf, plen, filename);
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( " z karty SD</p>\n" ) );
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;
        }
        if (cmd == URLstatus)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
          plen=printHTMLstatus(Enc28j60_global.buf, plen, Enc28j60_global.bufferSize);  
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;  
        }
      }
    }
    if ( Enc28j60_global.buf[IP_PROTO_P]==IP_PROTO_TCP && Enc28j60_global.buf[TCP_DST_PORT_H_P]==0 && Enc28j60_global.buf[TCP_DST_PORT_H_P]==MYTELNETPOERT_H 
      && (Enc28j60_global.buf[TCP_DST_PORT_L_P]>=MYTELNETPOERT_L || Enc28j60_global.buf[TCP_DST_PORT_L_P]<=MYTELNETPOERT_L + 20))
    {
      processIpPacket(Enc28j60_global.buf);
      continue;
    }
#endif