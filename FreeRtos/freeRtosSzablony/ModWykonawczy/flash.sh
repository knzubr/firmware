#!/usr/bin/expect -f

set dev "/dev/rs485tty"
set file "./rtosdemo.bin"

system "stty 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke < $dev"

spawn -open [ open $dev r+ ]
send_user "Calling bootloader\n"
send "bootaa"
expect "C"
send_user "Starting xmodem $dev\n"
close
system "sx -vv -o -b -X  $file > $dev < $dev"
send_user "\nOK ready\n"

