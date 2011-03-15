PROJECT(ModWykonawczy)

ADD_EXECUTABLE(modWykonawczy 
  main.c 
  hardware.c 
  )

add_library(headers 
  FreeRTOSConfig.h 
  main.h 
  hardware.h 
  )
