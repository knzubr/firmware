PROJECT(sterownikglowny)

ADD_EXECUTABLE(sterownikglowny main.c hardware.c protRs485.c serial.c vty.c enc_task.c spi.c diskio.c rtc.c)
add_library(headers            main.h hardware.h protRs485.h serial.h vty.h enc_task.h spi.h diskio.h rtc.h ../protocol1.h ffconf.h )
add_library(rtosLibs ../../freeRtos/Lib/enc28j60.c ../../freeRtos/Lib/include/enc28j60.h ../../freeRtos/Lib/ip_arp_udp_tcp.c ../../freeRtos/Lib/include/ip_arp_udp_tcp.h ../../freeRtos/Lib/xmodem.c ../../freeRtos/Lib/include/xmodem.h ../../freeRtos/Lib/vt100.c ../../freeRtos/Lib/include/vt100.h ../../freeRtos/Lib/cmdline.c ../../freeRtos/Lib/include/cmdline.h ../../freeRtos/Lib/ramdysk.c ../../freeRtos/Lib/include/ramdysk.h ../../freeRtos/Lib/ff.c)
