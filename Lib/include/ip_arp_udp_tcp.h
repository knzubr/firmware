/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 *********************************************/
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H
#include <avr/pgmspace.h>

#include "enc_task.h"


///Calculates CRC 16 bit checksum
///
/// uint8_t* pointer to the buffer
/// uint16_t buffer length
/// uint8_t  type
/// return CRC
uint16_t checksum                    (uint8_t *buf1, uint16_t len, uint8_t type);
void     make_eth                    (uint8_t *buf1);
void     fill_ip_hdr_checksum        (uint8_t *buf1);
void     make_ip                     (uint8_t *buf1);
void     make_tcphead                (uint8_t *buf1,uint16_t rel_ack_num,uint8_t mss,uint8_t cp_seq);

// you must call this function once before you use any of the other functions:
void     init_ip_arp_udp_tcp         (uint8_t *mymac,uint8_t *myip,uint8_t wwwp);

uint8_t  eth_type_is_arp_and_my_ip   (uint8_t *buf1,uint16_t len);
uint8_t  eth_type_is_ip_and_my_ip    (uint8_t *buf1,uint16_t len);
void     make_arp_answer_from_request(uint8_t *buf1);
void     make_echo_reply_from_request(uint8_t *buf1,uint16_t len);
void     make_udp_reply_from_request (uint8_t *buf1,char *data,uint8_t datalen,uint16_t port);

void     make_tcp_synack_from_syn    (uint8_t *buf1);
void     init_len_info               (uint8_t *buf1);
uint16_t get_tcp_data_pointer        (void);
uint16_t fill_tcp_data_p             (uint8_t *buf1,uint16_t pos, const prog_char *progmem_s);
uint16_t fill_tcp_data               (uint8_t *buf1,uint16_t pos, const char *s);
void     make_tcp_ack_from_any       (uint8_t *buf1);
void     make_tcp_ack_with_data      (uint8_t *buf1, uint16_t dlen);


#endif /* IP_ARP_UDP_TCP_H */
