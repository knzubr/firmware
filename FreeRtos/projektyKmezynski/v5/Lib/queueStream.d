queueStream.o queueStream.d : Lib/queueStream.c Lib/include/queueStream.h \
  Source/include/FreeRTOS.h Source/include/projdefs.h FreeRTOSConfig.h \
  Source/include/portable.h \
  Source/include/../../portable/GCC/ATMega323/portmacro.h \
  Source/include/mpu_wrappers.h Source/include/queue.h
