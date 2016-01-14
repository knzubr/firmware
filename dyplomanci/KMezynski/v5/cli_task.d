cli_task.o cli_task.d : cli_task.c cli_task.h Source/include/FreeRTOS.h \
  Source/include/projdefs.h FreeRTOSConfig.h Source/include/portable.h \
  Source/include/../../portable/GCC/ATMega323/portmacro.h \
  Source/include/mpu_wrappers.h Source/include/queue.h \
  Lib/include/cmdline.h softwareConfig.h
