#ifndef LANG_VTY
#define LANG_VTY PL

prog_char errorOK[]                                   = "Wszystko poprawnie\r\n";
prog_char errorNoFile[]                               = "Brak pliku\r\n";
prog_char errorxModemFrameStartTimeout[]              = "\r\n";
prog_char errorxModemByteSendTimeout[]                = "\r\n";
prog_char errorxModemWrongFrameNo[]                   = "\r\n";
prog_char errorxModemFrameFrameNoCorrectionNotMatch[] = "\r\n";
prog_char errorxModemFrameCrc[]                       = "xModem CRC error\r\n";
prog_char errorxModemRemoteSideCan[]                  = "Strona zdalna przerwala transmisje na ramce nr %d\r\n";
prog_char errorxModemUnknownResponse[]                = "xModem nieznana odpowiedx 0x%x\r\n";
prog_char errorBootloaderNotResponding[]              = "Bootloader nie odpowiada\r\n";

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

prog_char cmd_settime[]   = "ustawczas";        prog_char cmd_help_settime[]   = "[h] [m] [s] ustawia czas w formacie 24h format";
prog_char cmd_ac[]        = "ac";               prog_char cmd_help_ac[]        = "[wejscie 0-7] czyta analogowa wartosc na wejsciu przetwornika";

#else
#error "You can't add two language files. Remove #include "vty_*.h" line in file vty.c"
#endif
