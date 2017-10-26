#!/bin/bash
minicom -D /dev/ttyACM0 -b 500000 &
echo $! > minicom.pid


