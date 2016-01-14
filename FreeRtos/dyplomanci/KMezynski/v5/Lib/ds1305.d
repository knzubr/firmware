ds1305.o ds1305.d : Lib/ds1305.c Lib/include/ds1305.h Lib/include/spi.h \
  Source/include/FreeRTOS.h Source/include/projdefs.h FreeRTOSConfig.h \
  Source/include/portable.h \
  Source/include/../../portable/GCC/ATMega323/portmacro.h \
  Source/include/mpu_wrappers.h Source/include/semphr.h \
  Source/include/queue.h Source/include/queue.h Source/include/task.h \
  Source/include/list.h hardwareConfig.h Lib/include/spi.h
