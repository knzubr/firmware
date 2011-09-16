#ifndef SERIAL_H
#define SERIAL_H

#define RS485Enable()      (PORTD |=  0x08)
#define RS485Disable()     (PORTD &=  0xF7)

void WriteCom(unsigned char dat);
uint8_t ReadCom_withLimitedWaiting(void);

#endif /* SERIAL_H */
