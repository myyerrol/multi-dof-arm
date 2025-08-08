#ifndef STS3215_SERIAL_H
#define STS3215_SERIAL_H

#include <stdint.h>

void STS3215_FlushSerialRecvBuf(void);
void STS3215_FlushSerialTranBuf(void);
int  STS3215_ReadSerial(uint8_t *dat, int dat_len);
int  STS3215_WriteSerial(uint8_t *dat, int dat_len);
int  STS3215_WriteSerialByte(uint8_t dat_byt);

#endif
