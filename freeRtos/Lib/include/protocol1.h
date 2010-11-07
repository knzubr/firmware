// Addressing
#define FIRST_ROLLER_DRIVER_ADDR 1
#define LAST_ROLLER_DRIVER_ADDR  31
#define FIRST_LIGHT_DRIVER_ADDR  32
#define LAST_LIGHT_DRIVER_ADDR   63
#define FIRST_SENSOR_ADDR        64
#define LAST_SENSOR_ADDR         95

// General message
#define HDR_LEN         4
//  + header                             + data                + CRC           +
//  +------+----------+---------+--------+-------+     +-------+-------+-------+
//  | SYNC | op. code | address | length | data1 | ... | dataN | CrcHi | CrcLo |
//  +------+----------+---------+--------+-------+     +-------+-------+-------+
//
// hello response
#define HELLO_RESP_STATE1  0
#define HELLO_RESP_STATE2  1
#define HELLO_RESP_VER     2
#define HELLO_RESP_VER_LEN 5
#define HELLO_RESP_LEN     2+HELLO_RESP_VER_LEN


//  +------+----------+---------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+
//  | SYNC | rHELLO   | address | length | data1  | data2  | data3 | data4 | data5 | data6 | data7 | CrcHi | CrcLo |
//  |      |          |    0    |    7   | state1 | state2 |version|version|version|version|version|       |       |
//  |      |          |         |        |        |        |   v   |       |  .    |       |       |       |       |
//  +------+----------+---------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+
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
