#!/bin/bash
avr-objcopy -O ihex -R .eeprom build/firmware firmware.hex
make -f MakefileProgram program
