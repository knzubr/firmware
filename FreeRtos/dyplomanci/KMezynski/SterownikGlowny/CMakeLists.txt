PROJECT(sterownikglowny)

ADD_EXECUTABLE(sterownikglowny main.c                              main.h
                               configuration.c                     configuration.h
                               hardware.c                          hardware.h
                               hardwareConfig.h
                               serial.c                            serial.h
                               vty.c                               vty.h vty_pl.h vty_en.h  
                               sensors_task.c                      sensors_task.h
                               enc_task.c                          enc_task.h
                               )
add_library(Protocols          ../../freeRtos/Lib/vt100.c          ../../freeRtos/Lib/include/vt100.h            # Obsługa sekwencji stosowanytch w VT100
                               ../../freeRtos/Lib/cmdline.c        ../../freeRtos/Lib/include/cmdline.h          # Obsługa interpretera poleceń
                               ../../freeRtos/Lib/xmodem.c         ../../freeRtos/Lib/include/xmodem.h           # Obsługa systemu transmisji Xmodem
                               ../../freeRtos/Lib/Rs485_prot.c     ../../freeRtos/Lib/include/Rs485_prot.h       # Obsługa protokołu komunikującego się przez magistralę Rs485 z modułami wykonawczymi                                                                   ../../freeRtos/Lib/include/protocol1.h        # Rs485 Protocol constans
                               )
add_library(Hardware           ../../freeRtos/Lib/spi.c            ../../freeRtos/Lib/include/spi.h              # Obsługa magistrali SPI
                               ../../freeRtos/Lib/memory_x.c       ../../freeRtos/Lib/include/memory_x.h         # Obsługa malloc. Zlicza wolną i zajętą pamięć na stercie.
                               ../../freeRtos/Lib/mpc23s17.c       ../../freeRtos/Lib/include/mpc23s17.h         # Obsługa ekspandera portu
                               ../../freeRtos/Lib/mcp3008.c        ../../freeRtos/Lib/include/mcp3008.h          # Obsługa przetwornika A/C
                               ../../freeRtos/Lib/ds1305.c         ../../freeRtos/Lib/include/ds1305.h           # Obsługa zegara RTC
                               ../../freeRtos/Lib/enc28j60.c       ../../freeRtos/Lib/include/enc28j60.h         # Obsługa sterownika ethernetowego
                               )
add_library(NetworkStack       ../../freeRtos/Lib/net/ip.c         ../../freeRtos/Lib/net/include/ip.h
                               ../../freeRtos/Lib/net/arp.c        ../../freeRtos/Lib/net/include/arp.h
                               ../../freeRtos/Lib/net/net.c        ../../freeRtos/Lib/net/include/net.h
                               ../../freeRtos/Lib/net/netstack.c   ../../freeRtos/Lib/net/include/netstack.h
                               ../../freeRtos/Lib/net/icmp.c       ../../freeRtos/Lib/net/include/icmp.h
                               ../../freeRtos/Lib/net/socket.c     ../../freeRtos/Lib/include/net/socket.h
                               )
add_library(DoPosortowania     ../../freeRtos/Lib/ip_arp_udp_tcp.c ../../freeRtos/Lib/include/ip_arp_udp_tcp.h
                               diskio.c                            diskio.h
                               rtc.c                               rtc.h
                               )
add_library(Filesystems        ../../freeRtos/Lib/ramdysk.c        ../../freeRtos/Lib/include/ramdysk.h          # Obsługa systemu plików Fat8
                               ../../freeRtos/Lib/ff.c                                                           # Obcługa systemu plików Fat32
                               ffconf.h
                               )

