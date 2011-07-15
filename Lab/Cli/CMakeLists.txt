PROJECT(sterownikglowny)

ADD_EXECUTABLE(sterownikglowny main.c                                  main.h
                               configuration.c                         configuration.h                                   # Load / save configuration
                               hardware.c                              hardware.h
                               serial.c                                serial.h
                               vty.c                                   vty.h  
                               sensors_task.c                          sensors_task.h
                               netstack_task.c                         netstack_task.h
                               cli_task.c                              cli_task.h
                               )
add_library(Lang
                               vty_pl.h                                vty_en.h
                               Rs485_prot_pl.h                         Rs485_prot_en.h
                               hardware_pl.h                           hardware_en.h
                               )
add_library(Config
                               hardwareConfig.h                                                                          # Hardware config
                               FreeRTOSConfig.h                                                                          # FreeRTOSConfig.h
                               softwareConfig.h                                                                          # Software configuration ie. language
                               ffconf.h                                                                                  # Petits File system config
                               )
add_library(Protocols STATIC
                               ../../freeRtos/Lib/vt100.c              ../../freeRtos/Lib/include/vt100.h                # Obsługa sekwencji stosowanytch w VT100
                               ../../freeRtos/Lib/cmdline.c            ../../freeRtos/Lib/include/cmdline.h              # Obsługa interpretera poleceń
                               ../../freeRtos/Lib/xmodem.c             ../../freeRtos/Lib/include/xmodem.h               # Obsługa systemu transmisji Xmodem
                               ../../freeRtos/Lib/Rs485_prot.c         ../../freeRtos/Lib/include/Rs485_prot.h           # Obsługa protokołu komunikującego się przez magistralę Rs485 z modułami wykonawczymi  
                                                                       ../../freeRtos/Lib/include/protocol1.h            # Rs485 Protocol constans
                               )
add_library(FreeRtos STATIC
                                                                       ../../freeRtos/Source/include/FreeRTOS.h  
                                                                       ../../freeRtos/Source/include/semphr.h
                                                                       ../../freeRtos/Source/include/portable.h 
                               ../../freeRtos/Source/croutine.c        ../../freeRtos/Source/include/croutine.h  
                               ../../freeRtos/Source/list.c            ../../freeRtos/Source/include/list.h  
                               ../../freeRtos/Source/queue.c           ../../freeRtos/Source/include/queue.h  
                               ../../freeRtos/Source/tasks.c           ../../freeRtos/Source/include/task.h
                               ../../freeRtos/Lib/queueStream.c        ../../freeRtos/Lib/include/queueStream.h          # obsługa strumienia FILE na potrzeby biblioteki libc
                               )
add_library(Hardware STATIC
                               ../../freeRtos/Lib/spi.c                ../../freeRtos/Lib/include/spi.h                  # Obsługa magistrali SPI
                               ../../freeRtos/Lib/memory_x.c           ../../freeRtos/Lib/include/memory_x.h             # Obsługa malloc. Zlicza wolną i zajętą pamięć na stercie.
                               ../../freeRtos/Lib/mpc23s17.c           ../../freeRtos/Lib/include/mpc23s17.h             # Obsługa ekspandera portu
                               ../../freeRtos/Lib/mcp3008.c            ../../freeRtos/Lib/include/mcp3008.h              # Obsługa przetwornika A/C
                               ../../freeRtos/Lib/mcp4150.c            ../../freeRtos/Lib/include/mcp4150.h              # Obsługa przetwornika A/C
                               ../../freeRtos/Lib/ds1305.c             ../../freeRtos/Lib/include/ds1305.h               # Obsługa zegara RTC
                               ../../freeRtos/Lib/enc28j60.c           ../../freeRtos/Lib/include/enc28j60.h             # Obsługa sterownika ethernetowego
                               ../../freeRtos/Lib/sd_diskio.c          ../../freeRtos/Lib/include/sd_diskio.h            # Obsługa karty SD 
                               )
add_library(NetworkStack STATIC      
                               ../../freeRtos/Lib/net/nic.c            ../../freeRtos/Lib/net/include/nic.h              # Hardware abstraction 
                               ../../freeRtos/Lib/net/net.c            ../../freeRtos/Lib/net/include/net.h              # Network frames, packets, etc.
                               ../../freeRtos/Lib/net/ip.c             ../../freeRtos/Lib/net/include/ip.h               # IPv4 support
                               ../../freeRtos/Lib/net/arp.c            ../../freeRtos/Lib/net/include/arp.h              # Arp support
                               ../../freeRtos/Lib/net/icmp.c           ../../freeRtos/Lib/net/include/icmp.h             # Ping
                               ../../freeRtos/Lib/net/tcp.c            ../../freeRtos/Lib/net/include/tcp.h              # Tcp protocol
                               ../../freeRtos/Lib/net/udp.c            ../../freeRtos/Lib/net/include/udp.h              # Udp protocol
                               )
add_library(DoPosortowania
                               ../../freeRtos/Lib/staryStack.c.c       ../../freeRtos/Lib/include/staryStack.h.h
                               diskio.c                               
                               rtc.c                                   rtc.h
                               )
add_library(Filesystems STATIC
                               ../../freeRtos/Lib/ramdysk.c            ../../freeRtos/Lib/include/ramdysk.h              # Obsługa systemu plików Fat8
                               ../../freeRtos/Lib/ff.c                 ../../freeRtos/Lib/include/ff.h                   # Obsługa systemu plików Fat32
                               ../../freeRtos/Lib/include/integer.h
                               )
