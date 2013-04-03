cli_task.o cli_task.d : cli_task.c cli_task.h \
 ../../freeRtos/Source/include/FreeRTOS.h \
 ../../freeRtos/Source/include/projdefs.h FreeRTOSConfig.h \
 ../../freeRtos/Source/include/portable.h \
 ../../freeRtos/Source/include/../../portable/GCC/ATMega323/portmacro.h \
 ../../freeRtos/Source/include/mpu_wrappers.h \
 ../../freeRtos/Source/include/queue.h \
 ../../freeRtos/Lib/include/cmdline.h softwareConfig.h
