#ifndef LANG_VTY
#define LANG_VTY PL

// *************************** Error Strings *******************************************************

prog_char errorOK[]                                   = "Wszystko poprawnie\r\n";
prog_char errorNoFile[]                               = "Brak pliku\r\n";
prog_char errorxModemFrameStartTimeout[]              = "\r\n";
prog_char errorxModemByteSendTimeout[]                = "\r\n";
prog_char errorxModemWrongFrameNo[]                   = "\r\n";
prog_char errorxModemFrameFrameNoCorrectionNotMatch[] = "\r\n";
prog_char errorxModemFrameCrc[]                       = "xModem CRC error\r\n";
prog_char errorxModemRemoteSideCan[]                  = "Strona zdalna przerwala transmisje na ramce nr %d\r\n";
prog_char errorxModemUnknownResponse[]                = "xModem nieznana odpowiedx 0x%x\r\n";
prog_char errorNoRemoteDevice[]                       = "Urządzenie %d nie odpowiada (%d)\r\n";
prog_char errorBootloaderNotResponding[]              = "Bootloader nie odpowiada\r\n";
prog_char errorOpenFile[]                             = "Nie mozna otworzyc pliku %s\r\n";

// *************************** Message Strings *****************************************************

prog_char systemStateStr[]            = "Stan systemu:\r\n";
prog_char statusNumberOfTasksStr[]    = "  Liczba zadan        : %d\r\n";
prog_char statusStaticHeapStateStr[]  = "  Sterta dla FreeRtos : %d wolnych z %d bajtow\r\n";
prog_char statusDynamicHeapStateStr[] = "  Sterta dla malloc   : %d wolnych z %d bajtow\r\n";
prog_char statusRamDiskStateStr[]     = "  Ram dysk            : %d wolnych z %d klastrow\r\n";
prog_char statusTemperatureStr[]      = "  Temperatura         : %d C\r\n";
prog_char statusVoltageStr[]          = "  Napiecie            : %d V\r\n";
prog_char systemRamConfigStr[]        = "Ustawienia systemu:\r\n";
prog_char statusMacStr[]              = "  Adres MAC           : %x:%x:%x:%x:%x:%x\r\n";
prog_char statusIpStr[]               = "  Adres IP / maska    : %d.%d.%d.%d / %d\r\n";

prog_char statusRs485listStr[]        = "Wykryte urzadzenia na magistrali RS 485:\r\n";
prog_char statusRollerDescStr[]       = "  Adres %d firmware %c%c%c%c%c\r\n";

prog_char editRamFileIntroStr[]       = "Zapis do pliku. CTRL+C koniec\r\n";
prog_char readRamFIleLenStr[]         = "Dlugosc pliku: %d\r\n";

prog_char xwyslijStartStr[]           = "Xmodem: rozpoczynanie wysylania\r\n";

prog_char movingCurtainUpStr[]        = "Podnoszenie rolety\r\n\tsterownik %d\r\n\troleta    %d\r\n";
prog_char movingCurtainDownStr[]      = "Opuszczanie rolety\r\n\tsterownik %d\r\n\troleta    %d\r\n";
prog_char movingCurtainPosStr[]       = "\tpozycja   %d\r\n";

// *************************** Command Strings *****************************************************

prog_char cmd_help[]      = "pomoc";            prog_char cmd_help_help[]      = "Wypisuje wszystkie komendy";
prog_char cmd_status[]    = "status";           prog_char cmd_help_status[]    = "Wypisuje status urzadzenia";
prog_char cmd_time[]      = "czas";             prog_char cmd_help_time[]      = "Wypisuje czas";

prog_char cmd_ping[]      = "ping";             prog_char cmd_help_ping[]      = "[numer sterownika] Wysyla ping do sterownika podpietego do magistrali Rs485";
prog_char cmd_xRec[]      = "xodb";             prog_char cmd_help_xRec[]      = "[nazwa pliku] odbiera plik za pomoca protokolu xModem";
prog_char cmd_xSend[]     = "xwyslij";          prog_char cmd_help_xSend[]     = "[nazwa pliku] wysyla plik za pomoca protokolu xModem";
prog_char cmd_xflash[]    = "xflash";           prog_char cmd_help_xflash[]    = "[numer urzadzenia] [nazwa pliku] wgrywa firmware do urzadzenia podpietego do Rs485";
#ifdef testZewPamiec
prog_char cmd_rtest[]     = "rtest";            prog_char cmd_help_rtest[]     = "Test pamieci zewnetrznej";
#endif
prog_char cmd_dir_rf[]    = "dirrp" ;           prog_char cmd_help_dir_rf[]    = "Wypisuje nazwy plikow zapisanych w pamieci RAM";
prog_char cmd_create_rf[] = "utwrp";            prog_char cmd_help_create_rf[] = "[nazwa pliku] Tworzy plik w pamieci RAM";
prog_char cmd_erase_rf[]  = "kasrp";            prog_char cmd_help_erase_rf[]  = "[nazwa pliku] Usuwa plik z pamieci RAM";
prog_char cmd_edit_rf[]   = "dopiszrp";         prog_char cmd_help_edit_rf[]   = "[nazwa pliku] Dopisuje do pliku zapisanego w pamieci ram";
prog_char cmd_read_rf[]   = "czytrp";           prog_char cmd_help_read_rf[]   = "[nazwa pliku] Wypisuje zawartosc pliku";

prog_char cmd_up[]        = "podnies";          prog_char cmd_help_up[]        = "[numer sterownika] [nr rolety] {wartosc} podnoszenie rolety";
prog_char cmd_down[]      = "opusc";            prog_char cmd_help_down[]      = "[numer sterownika] [nr rolety] {wartosc} opuszczanie rolety";
prog_char cmd_spa[]       = "spa";              prog_char cmd_help_spa[]       = "[wartosc] ustaw zewnetrzny port A";

prog_char cmd_settime[]   = "ustawcz";          prog_char cmd_help_settime[]   = "[h] [m] [s] ustawia czas w formacie 24h format";
prog_char cmd_ac[]        = "ac";               prog_char cmd_help_ac[]        = "[wejscie 0-7] czyta analogowa wartosc na wejsciu przetwornika";
prog_char cmd_enable[]    = "admin";            prog_char cmd_help_enable[]    = "Wejscie w tryb uprzywilejowany";
prog_char cmd_disable[]   = "normalny";         prog_char cmd_help_disable[]   = "Wyjscie z trybu uprzywilejowanego";
prog_char cmd_configure[] = "konfig";           prog_char cmd_help_configure[] = "Wejscie w tryb konfiguracji";
prog_char cmd_conf_ip[]   = "ip";               prog_char cmd_help_conf_ip[]   = "[A1] [A2] [A2] [A3] [mask] ustaw adres IP i maske";
prog_char cmd_conf_mac[]  = "mac";              prog_char cmd_help_conf_mac[]  = "[A1] [A2] [A2] [A3] [A4] [A5] ustaw adres MAC";
prog_char cmd_conf_save[] = "zapisz";           prog_char cmd_help_conf_save[] = "Zapisz konfiguracje";

#else
#error "You can't add two language files. Remove #include "vty_*.h" line in file vty.c"
#endif
