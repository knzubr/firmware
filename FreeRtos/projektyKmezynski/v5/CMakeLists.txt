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
                               Lib/vt100.c              Lib/include/vt100.h                # Obsługa sekwencji stosowanytch w VT100
                               Lib/cmdline.c            Lib/include/cmdline.h              # Obsługa interpretera poleceń
                               Lib/xmodem.c             Lib/include/xmodem.h               # Obsługa systemu transmisji Xmodem
                               Lib/Rs485_prot.c         Lib/include/Rs485_prot.h           # Obsługa protokołu komunikującego się przez magistralę Rs485 z modułami wykonawczymi  
                                                                       Lib/include/protocol1.h            # Rs485 Protocol constans
                               )
add_library(FreeRtos STATIC
                                                                       Source/include/FreeRTOS.h  
                                                                       Source/include/semphr.h
                                                                       Source/include/portable.h 
                               Source/croutine.c        Source/include/croutine.h  
                               Source/list.c            Source/include/list.h  
                               Source/queue.c           Source/include/queue.h  
                               Source/tasks.c           Source/include/task.h
                               Lib/queueStream.c        Lib/include/queueStream.h          # obsługa strumienia FILE na potrzeby biblioteki libc
                               )
add_library(Hardware STATIC
                               Lib/spi.c                Lib/include/spi.h                  # Obsługa magistrali SPI
                               Lib/memory_x.c           Lib/include/memory_x.h             # Obsługa malloc. Zlicza wolną i zajętą pamięć na stercie.
                               Lib/mpc23s17.c           Lib/include/mpc23s17.h             # Obsługa ekspandera portu
                               Lib/mcp3008.c            Lib/include/mcp3008.h              # Obsługa przetwornika A/C
                               Lib/mcp4150.c            Lib/include/mcp4150.h              # Obsługa przetwornika A/C
                               Lib/ds1305.c             Lib/include/ds1305.h               # Obsługa zegara RTC
                               Lib/enc28j60.c           Lib/include/enc28j60.h             # Obsługa sterownika ethernetowego
                               Lib/sd_diskio.c          Lib/include/sd_diskio.h            # Obsługa karty SD 
                               )
add_library(NetworkStack STATIC      
                               Lib/net/nic.c            Lib/net/include/nic.h              # Hardware abstraction 
                               Lib/net/net.c            Lib/net/include/net.h              # Network frames, packets, etc.
                               Lib/net/ip.c             Lib/net/include/ip.h               # IPv4 support
                               Lib/net/arp.c            Lib/net/include/arp.h              # Arp support
                               Lib/net/icmp.c           Lib/net/include/icmp.h             # Ping
                               Lib/net/tcp.c            Lib/net/include/tcp.h              # Tcp protocol
                               Lib/net/udp.c            Lib/net/include/udp.h              # Udp protocol
                               )
add_library(DoPosortowania
                               Lib/staryStack.c.c       Lib/include/staryStack.h.h
                               diskio.c                               
                               rtc.c                                   rtc.h
                               )
add_library(Filesystems STATIC
                               Lib/ramdysk.c            Lib/include/ramdysk.h              # Obsługa systemu plików Fat8
                               Lib/ff.c                 Lib/include/ff.h                   # Obsługa systemu plików Fat32
                               Lib/include/integer.h
                               )
