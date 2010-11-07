#ifndef LANG_VTY
#define LANG_VTY EN

// *************************** Error Strings *******************************************************

prog_char errorOK[]                                   = "All OK\r\n";
prog_char errorNoFile[]                               = "Brak pliku\r\n";
prog_char errorxModemFrameStartTimeout[]              = "\r\n";
prog_char errorxModemByteSendTimeout[]                = "\r\n";
prog_char errorxModemWrongFrameNo[]                   = "\r\n";
prog_char errorxModemFrameFrameNoCorrectionNotMatch[] = "\r\n";
prog_char errorxModemFrameCrc[]                       = "xModem CRC error\r\n";
prog_char errorxModemRemoteSideCan[]                  = "Remote side cancelled at frame no %d\r\n";
prog_char errorxModemUnknownResponse[]                = "xModem unknown response 0x%x\r\n";
prog_char errorNoRemoteDevice[]                       = "Device %d is not responding (%d)\r\n";
prog_char errorBootloaderNotResponding[]              = "Bootloader is not responding\r\n";
prog_char errorOpenFile[]                             = "Can't open file %s\r\n";

// *************************** Message Strings *****************************************************

prog_char systemStateStr[]            = "System state:\r\n";
prog_char statusNumberOfTasksStr[]    = "  Number of tasks : %d\r\n";
prog_char statusStaticHeapStateStr[]  = "  FreeRtos heap   : %d free of %d bytes\r\n";
prog_char statusDynamicHeapStateStr[] = "  Malloc heap     : %d free of %d bytes\r\n";
prog_char statusRamDiskStateStr[]     = "  Ram disc space  : %d free of %d clusters\r\n";
prog_char statusTemperatureStr[]      = "  Temperature     : %d C\r\n";
prog_char statusVoltageStr[]          = "  Voltage         : %d V\r\n";
prog_char systemRamConfigStr[]        = "System settings:\r\n";
prog_char statusMacStr[]              = "  Mac address     : %x:%x:%x:%x:%x:%x\r\n";
prog_char statusIpStr[]               = "  IP addr./ mask  : %d.%d.%d.%d / %d\r\n";

prog_char statusRs485listStr[]        = "Detected RS 485 devices:\r\n";
prog_char statusNoRs485Dev[]          = "  Can't find any device\r\n";

prog_char statusLockerSensorsStr[]    = "Locker sensors states:\r\n";
prog_char statusLockerSensorsDisStr[] = "  Locker sensors disabled\r\n";

prog_char editRamFileIntroStr[]       = "Writing to file. Press CTRL+C to quit\r\n";
prog_char readRamFIleLenStr[]         = "File length: %d\r\n";

prog_char xwyslijStartStr[]           = "Xmodem: Transmission start\r\n";

prog_char movingCurtainUpStr[]        = "Podnoszenie rolety\r\n\tsterownik %d\r\n\troleta    %d\r\n";
prog_char movingCurtainDownStr[]      = "Opuszczanie rolety\r\n\tsterownik %d\r\n\troleta    %d\r\n";
prog_char movingCurtainPosStr[]       = "\tpozycja   %d\r\n";

// *************************** Command Strings *****************************************************

prog_char cmd_help[]      = "help";             prog_char cmd_help_help[]      = "Print help string";
prog_char cmd_status[]    = "status";           prog_char cmd_help_status[]    = "Print device status";
prog_char cmd_time[]      = "time";             prog_char cmd_help_time[]      = "Print time";

prog_char cmd_ping[]      = "ping";             prog_char cmd_help_ping[]      = "[Device no] Send ping to Rs485 device";
prog_char cmd_xRec[]      = "xrec";             prog_char cmd_help_xRec[]      = "[file name] receive file using xModem";
prog_char cmd_xSend[]     = "xsend";            prog_char cmd_help_xSend[]     = "[file name] send file using xModem";
prog_char cmd_xflash[]    = "xflash";           prog_char cmd_help_xflash[]    = "[device no] [file name] flash device connected to Rs485";
#ifdef testZewPamiec
prog_char cmd_rtest[]     = "rtest";            prog_char cmd_help_rtest[]     = "External ram test";
#endif
prog_char cmd_dir_rf[]    = "dirrf";            prog_char cmd_help_dir_rf[]    = "Print ramdisk files";
prog_char cmd_create_rf[] = "crf";              prog_char cmd_help_create_rf[] = "[file name] create ram file";
prog_char cmd_erase_rf[]  = "eraserf";          prog_char cmd_help_erase_rf[]  = "[file name] erase file from ram disk";
prog_char cmd_edit_rf[]   = "editrf";           prog_char cmd_help_edit_rf[]   = "[file name] edit file located on ram disk";
prog_char cmd_read_rf[]   = "readrf";           prog_char cmd_help_read_rf[]   = "[file name] read file located on ram disk";

prog_char cmd_up[]        = "up";               prog_char cmd_help_up[]        = "[driver no] [channel] {value} move up";
prog_char cmd_down[]      = "down";             prog_char cmd_help_down[]      = "[driver no] [channel] {value} move down";
prog_char cmd_spa[]       = "spa";              prog_char cmd_help_spa[]       = "[value] set port A";

prog_char cmd_settime[]   = "settime";          prog_char cmd_help_settime[]   = "[h] [m] [s] set time (24h format)";
prog_char cmd_ac[]        = "ac";               prog_char cmd_help_ac[]        = "[channel 0-7] read analog value";
prog_char cmd_enable[]    = "enable";           prog_char cmd_help_enable[]    = "Enable mode";
prog_char cmd_disable[]   = "disable";          prog_char cmd_help_disable[]   = "View mode";
prog_char cmd_configure[] = "config";           prog_char cmd_help_configure[] = "Configure mode";
prog_char cmd_conf_ip[]   = "ip";               prog_char cmd_help_conf_ip[]   = "[A1] [A2] [A3] [A4] [mask] set IP address and mask";
prog_char cmd_conf_mac[]  = "mac";              prog_char cmd_help_conf_mac[]  = "[A1] [A2] [A2] [A3] [A4] [A5] set MAC address";
prog_char cmd_conf_save[] = "save";             prog_char cmd_help_conf_save[] = "Save configuration";

#endif