#include "falsh.h"



inline void write_one_page(unsigned char *buf) //Uaktualnianie strony z pamięcią flash
{
  int8_t pagptr;
  boot_page_erase(FlashAddr);                  //Kasowanie strony
  boot_spm_busy_wait();                        //Czekanie na wykonanie operacji
  
  uint16_t *tmpPtr = (uint16_t *)(buf);        //kompilator musi być little bit endian
  for(pagptr = 0; pagptr < SPM_PAGESIZE; pagptr += 2)
  {
    boot_page_fill(pagptr, *tmpPtr);          //brzydkie rzutowanie 2X uint8_t na uint16_t
    tmpPtr++;                                 //wskaźnik zwiększa się o 2
  }
  boot_page_write(FlashAddr);                  //Zapis strony
  boot_spm_busy_wait();                        //Czekanie na wykonanie operacji
}