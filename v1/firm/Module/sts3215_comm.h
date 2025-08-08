#ifndef STS3215_COMM_H
#define STS3215_COMM_H

#include <stdint.h>
#include <stdlib.h>

enum STS3215_ERR_LIST {
    STS3215_ERR_NO_REPLY = 1,
    STS3215_ERR_CRC_CMP  = 2,
    STS3215_ERR_SLAVE_ID = 3,
    STS3215_ERR_BUFF_LEN = 4,
};

int  STS3215_ReadByte1(uint8_t idx, uint8_t mem_addr);
int  STS3215_ReadByte2(uint8_t idx, uint8_t mem_addr);
int  STS3215_ReadNormal(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len);
void STS3215_ReadSyncBegin(uint8_t id_num, uint8_t rx_len);
void STS3215_ReadSyncEnd(void);
int  STS3215_WriteAsync(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len);
int  STS3215_WriteAsyncAction(uint8_t idx);
int  STS3215_writeByte1(uint8_t idx, uint8_t mem_addr, uint8_t dat_byt_1);
int  STS3215_WriteByte2(uint8_t idx, uint8_t mem_addr, uint16_t dat_byt_2);
int  STS3215_WriteNormal(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len);
void STS3215_WriteSync(uint8_t idx[], uint8_t id_len, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len);

int  STS3215_DecoReadPacketByte1(void);
int  STS3215_DecoReadPacketByte2(uint8_t neg_bit);
int  STS3215_Ping(uint8_t idx);
int  STS3215_Reset(uint8_t idx);
int  STS3215_RecvReadSyncPacket(uint8_t idx, uint8_t *dat);
int  STS3215_TranReadSyncPacket(uint8_t idx[], uint8_t id_len, uint8_t mem_addr, uint8_t dat_len);

int  STS3215_Ack(uint8_t idx);
int  STS3215_CheckHead(void);
int  STS3215_CvrtByteToWord(uint8_t dat_l, uint8_t dat_h);
void STS3215_CvrtWordToByte(uint8_t *dat_l, uint8_t *dat_h, int dat);
void STS3215_WriteBuf(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len, uint8_t fun);

uint8_t STS3215_GetEndian(void);
int     STS3215_GetStatusComm(void);
int     STS3215_GetStatusServo(void);
void    STS3215_SetEndian(uint8_t end);
void    STS3215_SetLevel(uint8_t lev);

#endif
