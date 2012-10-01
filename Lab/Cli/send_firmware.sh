./eth-bootloader/udpClientReset/udpConsole $1 2995 3000
#sleep 1
hostname=$1
flash_filename=$2
eeprom_filename=$3
tftp $hostname<</dev/null

timeout 10
rexmt 1
put $flash_filename
put $eeprom_filename
quit
