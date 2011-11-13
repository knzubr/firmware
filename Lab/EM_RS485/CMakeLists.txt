PROJECT(Korutyny)
SET(CMAKE_SYSTEM_NAME "Korutyny i magistrala Rs485")

cmake_minimum_required(VERSION 2.8)

SET(MCU               atmega168)
SET(PROGRAMMER_DEV    /dev/avrMultiTool)

SET(IMAGENAME         firmware)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

# Output format. (can be srec, ihex, binary)
SET(FORMAT       "ihex")

# Target file name (without extension).
SET(TARGET_NAME  ${IMAGENAME})


SET(SOURCE_DIR   "../../freeRtos/Source")
SET(PORT_DIR     "../../freeRtos/portable/GCC/ATMega88")
SET(PORT_MEM     "../../freeRtos/portable/MemMang")

include_directories("./" "${SOURCE_DIR}/include" "${PORT_DIR}")

SET(CSTANDARD    "-std=gnu99")
SET(CDEBUG       "-gstabs")
SET(CWARN        "-Wall -Wstrict-prototypes")
SET(CTUNING      "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT         "-Os")
SET(CINCS        "-I. -I${SOURCE_DIR}/include -I${LIB_DIR}/include -I${LIB_NET_DIR}/include")
SET(CMCU         "-mmcu=${MCU}")
SET(CDEFS        "-DF_CPU=7372800")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})


add_executable(${IMAGENAME}
                        main.c hardware.c 
                        ${SOURCE_DIR}/tasks.c ${SOURCE_DIR}/queue.c ${SOURCE_DIR}/list.c ${SOURCE_DIR}/croutine.c 
                        ${PORT_MEM}/heap_avr.c
                        ${PORT_DIR}/port.c )

ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.hex
    COMMAND avr-objcopy -O ${FORMAT} -R .eeprom ${IMAGENAME} ${IMAGENAME}.hex
    DEPENDS ${IMAGENAME}
)
 
ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.eep
    COMMAND avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ${FORMAT} ${IMAGENAME} ${IMAGENAME}.eep
    DEPENDS ${IMAGENAME}
)

add_custom_target(install
    COMMAND avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c jtag1 -U flash:w:${IMAGENAME}.hex -U eeprom:w:${IMAGENAME}.eep
    DEPENDS ${IMAGENAME}.hex ${IMAGENAME}.eep
    COMMENT "Upload firmware to microcontroller"
)
