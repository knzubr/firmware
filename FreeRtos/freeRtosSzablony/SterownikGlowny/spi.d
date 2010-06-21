spi.o spi.d : spi.c ../../freeRtos/Source/include/FreeRTOS.h \
  ../../freeRtos/Source/include/projdefs.h FreeRTOSConfig.h \
  ../../freeRtos/Source/include/portable.h \
  ../../freeRtos/Source/include/../../portable/GCC/ATMega323/portmacro.h \
  ../../freeRtos/Source/include/mpu_wrappers.h \
  ../../freeRtos/Source/include/queue.h \
  ../../freeRtos/Source/include/task.h \
  ../../freeRtos/Source/include/list.h hardware.h spi.h main.h \
  ../../freeRtos/Source/include/croutine.h serial.h \
  ../../freeRtos/Source/include/semphr.h \
  ../../freeRtos/Source/include/queue.h \
  ../../freeRtos/Lib/include/cmdline.h vty.h protRs485.h \
  ../../freeRtos/Lib/include/ramdysk.h main.h
