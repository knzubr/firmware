/**
 * @file      arp.c
 * @version   0.2
 * @author    Pascal Stang, Adam Kaliszan
 * \brief     ARP Protocol Library. 
 *
 * Created       :  7.09.2004
 * Revised       : 29.11.2010
 * Editor Tabs   : 4
 *
 */


#include "arp.h"


/// Single ARP table entry/record
struct ArpEntry
{
  uint32_t ipaddr;                           ///< remote-note IP address
  struct netEthAddr ethaddr;                 ///< remote-node ethernet (hardware/mac) address
  uint8_t time;                              ///< time to live (in ARP table); this is decremented by arpTimer()
};

// global variables
struct ArpEntry ArpTable[ARP_TABLE_SIZE];    ///< ARP table of matched IP<->MAC associations

#ifdef ARP_DEBUG

void setArpDebug(FILE *stream, uint8_t level)
{
  arpDebug = stream;
  arpDebugLevel = level;
  if (level == 0)
    arpDebug = NULL;
}
#endif /*ARP_DEBUG*/

void arpInit()
{

  memset(ArpTable, 0, sizeof(ArpTable));
  arpDebug = NULL;
}

void arpArpIn(unsigned int len, struct netEthArpHeader* packet)
{
#ifdef ARP_DEBUG
  if (arpDebug != NULL)
  {
    if (arpDebugLevel > 1)
      fprintf_P(arpDebug, PSTR("Received ARP Request\r\n"));
    if (arpDebugLevel > 2)
      arpPrintHeader(arpDebug, &packet->arp );   
  }
#endif

// for now, we just reply to requests
// need to add ARP cache
  if( (packet->arp.dipaddr == IpMyConfig.ip) && (packet->arp.opcode == htons(ARP_OPCODE_REQUEST)) )
  {
// in ARP header
// copy sender's address info to dest. fields
    packet->arp.dhwaddr = packet->arp.shwaddr;
    packet->arp.dipaddr = packet->arp.sipaddr;
// fill in our information
    packet->arp.shwaddr =  nicState.mac;
    packet->arp.sipaddr =  IpMyConfig.ip;
// change op to reply
    packet->arp.opcode = htons(ARP_OPCODE_REPLY);

// in ethernet header
    packet->eth.dest = packet->eth.src;
    packet->eth.src  = nicState.mac;

#ifdef ARP_DEBUG
    if (arpDebug != NULL)
    {
      if (arpDebugLevel > 0)
        fprintf_P(arpDebug, PSTR("Sending ARP Reply\r\n"));
      if (arpDebugLevel > 2)
        arpPrintHeader(arpDebug, &packet->arp );
    }
#endif
// send reply!
    nicSend(len);
  }
}

void arpIpIn(struct netEthIpHeader* packet)
{
#ifdef ARP_DEBUG
  if (arpDebug != NULL)
  {
    if (arpDebugLevel > 0)
    {
      fprintf_P(arpDebug, PSTR("ARP IP in MAC: "));
      netPrintEthAddr(arpDebug, &packet->eth.src);
      fprintf_P(arpDebug, PSTR(" IP: "));
      netPrintIPAddr(arpDebug, packet->ip.srcipaddr);
      fprintf_P(arpDebug, PSTR("\r\n"));
    }
  }
#endif
  int8_t index;

// check if sender is already present in arp table
  index = arpMatchIp(packet->ip.srcipaddr);
  if(index != -1)
  {
// sender's IP address found, update ARP entry
    ArpTable[index].ethaddr = packet->eth.src;
    ArpTable[index].time = ARP_CACHE_TIME_TO_LIVE;
// and we're done
    return;
  }

// sender was not present in table,
// must add in empty/expired slot
  for(index=0; index<ARP_TABLE_SIZE; index++)
  {
    if(ArpTable[index].time == 0)
    {
// write entry
      ArpTable[index].ethaddr = packet->eth.src;
      ArpTable[index].ipaddr = packet->ip.srcipaddr;
      ArpTable[index].time = ARP_CACHE_TIME_TO_LIVE;
// and we're done
      return;
    }
  }
// no space in table, we give up
}

void arpIpOut(struct netEthIpHeader* packet, uint32_t phyDstIp)
{
  int index;
// check if destination is already present in arp table
// use the physical dstIp if it's provided, otherwise the dstIp in packet
  if(phyDstIp)
    index = arpMatchIp(phyDstIp);
  else
    index = arpMatchIp(packet->ip.destipaddr);
// fill in ethernet info
  if(index != -1)
  {
// ARP entry present, fill eth address(es)
    packet->eth.src  = nicState.mac;
    packet->eth.dest = ArpTable[index].ethaddr;
    packet->eth.type = HTONS(ETHTYPE_IP);
  }
  else
  {
// not in table, must send ARP request
    packet->eth.src = nicState.mac;
// TODO MUST CHANGE, but for now, send this one broadcast
// before sending frame, must copy buffer
    packet->eth.dest.addr[0] = 0xFF;
    packet->eth.dest.addr[1] = 0xFF;
    packet->eth.dest.addr[2] = 0xFF;
    packet->eth.dest.addr[3] = 0xFF;
    packet->eth.dest.addr[4] = 0xFF;
    packet->eth.dest.addr[5] = 0xFF;
    packet->eth.type = HTONS(ETHTYPE_IP);
  }
}

void arpTimer(void)
{
  int index;
// this function meant to be called on a regular time interval

// decrement time-to-live for all entries
  for(index=0; index<ARP_TABLE_SIZE; index++)
  {
    if(ArpTable[index].time)
      ArpTable[index].time--;
  }
}

int arpMatchIp(uint32_t ipaddr)
{
  uint8_t i;

  // check if IP address is present in arp table
  for(i=0; i<ARP_TABLE_SIZE; i++)
  {
    if(ArpTable[i].ipaddr == ipaddr)
    {
// IP address found
      return i;
    }
  }

// no match
  return -1;
}

#if ARP_DEBUG
void arpPrintHeader(FILE *stream, struct netArpHeader* packet)
{
  fprintf_P(stream, PSTR("ARP Packet:\r\n"));
  //debugPrintHexTable(60, (unsigned char*)&packet);
  // print operation type
  fprintf_P(stream, PSTR("Operation   : "));
  if(packet->opcode == htons(ARP_OPCODE_REQUEST))
    fprintf_P(stream, PSTR("REQUEST"));
  else if(packet->opcode == htons(ARP_OPCODE_REPLY))
    fprintf_P(stream, PSTR("REPLY"));
  else
    fprintf_P(stream, PSTR("UNKNOWN"));
  fprintf_P(stream, PSTR("\r\n"));
// print source hardware address
  fprintf_P(stream, PSTR("SrcHwAddr   : "));  netPrintEthAddr(stream, &packet->shwaddr);   fprintf_P(stream, PSTR("\r\n"));
// print source protocol address
  fprintf_P(stream, PSTR("SrcProtoAddr: "));  netPrintIPAddr(stream, packet->sipaddr);     fprintf_P(stream, PSTR("\r\n"));
// print target hardware address
  fprintf_P(stream, PSTR("DstHwAddr   : "));  netPrintEthAddr(stream, &packet->dhwaddr);   fprintf_P(stream, PSTR("\r\n"));
// print target protocol address
  fprintf_P(stream, PSTR("DstProtoAddr: "));  netPrintIPAddr(stream, packet->dipaddr);     fprintf_P(stream, PSTR("\r\n"));
}
#endif /*ARP_DEBUG*/

void arpPrintTable(FILE *stream)
{
  uint8_t i;

  // print ARP table
  fprintf_P(stream, PSTR("Time Eth Address        IP Address\r\n"));
  fprintf_P(stream, PSTR("-----------------------------------\r\n"));
  fprintf_P(stream, PSTR(" MY  "));
  netPrintEthAddr(stream, &nicState.mac);
  fprintf_P(stream, PSTR("  "));
  netPrintIPAddr(stream, IpMyConfig.ip);
  fprintf_P(stream, PSTR("\r\n"));

  for(i=0; i<ARP_TABLE_SIZE; i++)
  {
    if (ArpTable[i].ipaddr == 0)
      continue;
    fprintf_P(stream, PSTR("%3d  "), ArpTable[i].time);
    netPrintEthAddr(stream, &ArpTable[i].ethaddr);
    fprintf_P(stream, PSTR("  "));
    netPrintIPAddr(stream, ArpTable[i].ipaddr);
    fprintf_P(stream, PSTR("\r\n"));
  }
}
