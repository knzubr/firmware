// General message
//  +------+----------+---------+--------+-------+     +-------+-------+-------+
//  | SYNC | op. code | address | length | data1 | ... | dataN | CrcHi | CrcLo |
//  +------+----------+---------+--------+-------+     +-------+-------+-------+
//
// hello response
//  +------+----------+---------+--------+----------------+-------+-------+-------+-------+-------+-------+-------+-------+
//  | SYNC | rHELLO   | address | length | data1          | data2 | data3 | data4 | data5 | data6 | data7 | CrcHi | CrcLo |
//  |      |          |    0    |    7   |mode(b/r/s/..)|my addr|version|version|version|version|version|       |       |
//  |      |          |         |        | b              |       |   v   |       |  .    |       |       |       |       |
//  +------+----------+---------+--------+----------------+-------+-------+-------+-------+-------+-------+-------+-------+
// mode:
//  b - bootloader
//  r - roller driver
//  s - sensor driver
// Version 5 bytes in ascii code


#ifndef PROTOCOL1_H
#define PROTOCOL1_H 1

#define SYNC              0x5A
#define rOpuscRolete1     0x10
#define rOpuscRolete2     0x11
#define rPodniesRolete1   0x20
#define rPodniesRolete2   0x21
#define rZatrzymajRolete1 0x30
#define rZatrzymajRolete2 0x31
#define rPING             0x80
#define rFLASH            0x81
#define rHELLO            0x82

#define rNIEZNANY         0xFF
#endif /* PROTOCOL1_H */
