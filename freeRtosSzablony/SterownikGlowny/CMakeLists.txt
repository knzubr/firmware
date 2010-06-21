PROJECT(sterownikglowny)

ADD_EXECUTABLE(sterownikglowny main.c hardware.c protRs485.c serial.c vty.c spi.c)
add_library(headers            main.h hardware.h protRs485.h serial.h vty.h spi.h )
add_library(rtosLibs ../../freeRtos/Lib/xmodem.c ../../freeRtos/Lib/xmodem.h ../../freeRtos/Lib/vt100.c ../../freeRtos/Lib/vt100.h ../../freeRtos/Lib/cmdline.c ../../freeRtos/Lib/cmdline.h ../../freeRtos/Lib/ramdysk.c ../../freeRtos/Lib/ramdysk.h ../../freeRtos/protocol1.h)
