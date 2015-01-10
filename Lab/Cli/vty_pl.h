#ifndef LANG_VTY
#define LANG_VTY PL

#include <avr/pgmspace.h>
// *************************** Error Strings *******************************************************

const char errorOK[]                                   PROGMEM = "Wszystko poprawnie\r\n";
const char errorNoFile[]                               PROGMEM = "Brak pliku\r\n";
const char errorxModemFrameStartTimeout[]              PROGMEM = "\r\n";
const char errorxModemByteSendTimeout[]                PROGMEM = "\r\n";
const char errorxModemWrongFrameNo[]                   PROGMEM = "\r\n";
const char errorxModemFrameFrameNoCorrectionNotMatch[] PROGMEM = "\r\n";
const char errorxModemFrameCrc[]                       PROGMEM = "xModem CRC error\r\n";
const char errorxModemRemoteSideCan[]                  PROGMEM = "Strona zdalna przerwala transmisje na ramce nr %d\r\n";
const char errorxModemUnknownResponse[]                PROGMEM = "xModem nieznana odpowiedx 0x%x\r\n";
const char errorNoRemoteDevice[]                       PROGMEM = "Urządzenie %d nie odpowiada (%d)\r\n";
const char errorBootloaderNotResponding[]              PROGMEM = "Bootloader nie odpowiada\r\n";
const char errorOpenFile[]                             PROGMEM = "Nie mozna otworzyc pliku %s\r\n";

// *************************** Message Strings *****************************************************

const char systemStateStr[]            PROGMEM = "Stan systemu:\r\n";
const char statusNumberOfTasksStr[]    PROGMEM = "  Liczba zadan        : %d\r\n";
const char statusStaticHeapStateStr[]  PROGMEM = "  Sterta dla FreeRtos : %d wolnych z %d bajtow\r\n";
const char statusDynamicHeapStateStr[] PROGMEM = "  Sterta dla malloc   : %d wolnych z %d bajtow\r\n";
const char statusRamDiskStateStr[]     PROGMEM = "  Ram dysk            : %d wolnych z %d klastrow\r\n";
const char statusTemperatureStr[]      PROGMEM = "  Temperatura         : %d C\r\n";
const char statusVoltageStr[]          PROGMEM = "  Napiecie            : %d V\r\n";
const char systemRamConfigStr[]        PROGMEM = "Ustawienia systemu:\r\n";
const char statusMacStr[]              PROGMEM = "  Adres MAC           : ";
const char statusIpStr[]               PROGMEM = "  IP adres            : ";
const char statusIpMaskStr[]           PROGMEM = "  maska               : ";
const char statusIpGwStr[]             PROGMEM = "  brama               : ";

const char statusRs485listStr[]        PROGMEM = "Wykryte urzadzenia na magistrali RS 485:\r\n";
const char statusNoRs485Dev[]          PROGMEM = "  Nie moge znale ani jednego urzadzenia\r\n";

const char statusLockerSensorsStr[]    PROGMEM = "Stan czujników rygli:\r\n";
const char statusLockerSensorsDisStr[] PROGMEM = "  Czujniki rygli wyłączone\r\n";

const char editRamFileIntroStr[]       PROGMEM = "Zapis do pliku. CTRL+C koniec\r\n";
const char readRamFIleLenStr[]         PROGMEM = "Dlugosc pliku: %d\r\n";

const char xwyslijStartStr[]           PROGMEM = "Xmodem: rozpoczynanie wysylania\r\n";

const char movingCurtainUpStr[]        PROGMEM = "Podnoszenie rolety\r\n\tsterownik %d\r\n\troleta    %d\r\n";
const char movingCurtainDownStr[]      PROGMEM = "Opuszczanie rolety\r\n\tsterownik %d\r\n\troleta    %d\r\n";
const char movingCurtainPosStr[]       PROGMEM = "\tpozycja   %d\r\n";

const char debugEnabledInfoStr[]       PROGMEM = "Wlaczono debugowanie %s\r\n";
const char debugDisabledInfoStr[]      PROGMEM = "Wylaczono debugowanie %s\r\n";

// *************************** Command Strings *****************************************************

const char cmd_help[]         PROGMEM = "pomoc";            const char cmd_help_help[]         PROGMEM = "Wypisuje wszystkie komendy";
const char cmd_status[]       PROGMEM = "status";           const char cmd_help_status[]       PROGMEM = "{nazwa pliku} Wypisuje status urzadzenia na ekranie lub zapisuje do pliku";
const char cmd_enc_stat[]     PROGMEM = "encstat";          const char cmd_help_enc_stat[]     PROGMEM = "Wypisz rejestry Enc 28j60";
const char cmd_time[]         PROGMEM = "czas";             const char cmd_help_time[]         PROGMEM = "Wypisuje czas";
const char cmd_net_dbg[]      PROGMEM = "debug";            const char cmd_help_net_dbg[]      PROGMEM = "[arp|icmp|ip|tcp|udp] [poziom] wypisywanie komunikatow. Poziom 0 wylacza komunikaty";

const char cmd_rping[]        PROGMEM = "rping";            const char cmd_help_rping[]        PROGMEM = "[numer sterownika] Wysyla ping do sterownika podpietego do magistrali Rs485";
const char cmd_ping[]         PROGMEM = "ping";             const char cmd_help_ping[]         PROGMEM = "[A1] [A2] [A3] [A4] Wysyla ping przez ethernet";
const char cmd_xRec[]         PROGMEM = "xodb";             const char cmd_help_xRec[]         PROGMEM = "[nazwa pliku] odbiera plik za pomoca protokolu xModem";
const char cmd_xSend[]        PROGMEM = "xwyslij";          const char cmd_help_xSend[]        PROGMEM = "[nazwa pliku] wysyla plik za pomoca protokolu xModem";
const char cmd_xflash[]       PROGMEM = "xflash";           const char cmd_help_xflash[]       PROGMEM = "[numer urzadzenia] [nazwa pliku] wgrywa firmware do urzadzenia podpietego do Rs485";
#ifdef testZewPamiec
const char cmd_rtest[]        PROGMEM = "rtest";            const char cmd_help_rtest[]        PROGMEM = "Test pamieci zewnetrznej";
#endif
const char cmd_dir_rf[]       PROGMEM = "dirrp" ;           const char cmd_help_dir_rf[]       PROGMEM = "Wypisuje nazwy plikow zapisanych w pamieci RAM";
const char cmd_create_rf[]    PROGMEM = "utwrp";            const char cmd_help_create_rf[]    PROGMEM = "[nazwa pliku] Tworzy plik w pamieci RAM";
const char cmd_erase_rf[]     PROGMEM = "kasrp";            const char cmd_help_erase_rf[]     PROGMEM = "[nazwa pliku] Usuwa plik z pamieci RAM";
const char cmd_edit_rf[]      PROGMEM = "dopiszrp";         const char cmd_help_edit_rf[]      PROGMEM = "[nazwa pliku] Dopisuje do pliku zapisanego w pamieci ram";
const char cmd_read_rf[]      PROGMEM = "czytrp";           const char cmd_help_read_rf[]      PROGMEM = "[nazwa pliku] Wypisuje zawartosc pliku";

const char cmd_up[]           PROGMEM = "podnies";          const char cmd_help_up[]           PROGMEM = "[numer sterownika] [nr rolety] {wartosc} podnoszenie rolety";
const char cmd_down[]         PROGMEM = "opusc";            const char cmd_help_down[]         PROGMEM = "[numer sterownika] [nr rolety] {wartosc} opuszczanie rolety";
const char cmd_spa[]          PROGMEM = "spa";              const char cmd_help_spa[]          PROGMEM = "[wartosc] ustaw zewnetrzny port A";
const char cmd_spb[]          PROGMEM = "spb";              const char cmd_help_spb[]          PROGMEM = "[wartosc] ustaw zewnetrzny port B";

const char cmd_settime[]      PROGMEM = "ustawcz";          const char cmd_help_settime[]      PROGMEM = "[h] [m] [s] ustawia czas w formacie 24h format";
const char cmd_ac[]           PROGMEM = "ac";               const char cmd_help_ac[]           PROGMEM = "[wejscie 0-7] czyta analogowa wartosc na wejsciu przetwornika";
const char cmd_enable[]       PROGMEM = "admin";            const char cmd_help_enable[]       PROGMEM = "Wejscie w tryb uprzywilejowany";
const char cmd_disable[]      PROGMEM = "normalny";         const char cmd_help_disable[]      PROGMEM = "Wyjscie z trybu uprzywilejowanego";
const char cmd_configure[]    PROGMEM = "konfig";           const char cmd_help_configure[]    PROGMEM = "Wejscie w tryb konfiguracji";
const char cmd_conf_ip[]      PROGMEM = "ip";               const char cmd_help_conf_ip[]      PROGMEM = "[A1] [A2] [A2] [A3] ustaw adres IP";
const char cmd_conf_udp[]     PROGMEM = "udp";              const char cmd_help_conf_udp[]     PROGMEM = "[A1] [A2] [A3] [A4] [src port] {dst port} ustaw adres klienta udp oraz porty";
const char cmd_conf_ip_mask[] PROGMEM = "maska";            const char cmd_conf_ip_mask_help[] PROGMEM = "[maska] ustaw maske adresu IP";
const char cmd_conf_ip_gw[]   PROGMEM = "brama";            const char cmd_conf_ip_gw_help[]   PROGMEM = "[A1] [A2] [A3] [A4] ustaw domyslna brame";
const char cmd_conf_mac[]     PROGMEM = "mac";              const char cmd_help_conf_mac[]     PROGMEM = "[A1] [A2] [A3] [A4] set MAC address";
const char cmd_conf_mac[]     PROGMEM = "mac";              const char cmd_help_conf_mac[]     PROGMEM = "[A1] [A2] [A3] [A4] [A5] [A6] ustaw adres MAC";
const char cmd_conf_save[]    PROGMEM = "zapisz";           const char cmd_help_conf_save[]    PROGMEM = "Zapisz konfiguracje";

#endif
