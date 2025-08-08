#include <sts3215_comm.h>
#include <sts3215_inst.h>
#include <sts3215_serial.h>

uint8_t  g_level = 1;
uint8_t  g_end   = 0;
uint8_t  g_status_servo;
uint8_t  g_status_comm;

uint8_t  g_read_sync_packet_idx;
uint8_t  g_read_sync_packet_len;
uint8_t *g_read_sync_packet;
uint8_t *g_read_sync_buf;
uint16_t g_read_sync_buf_len;
uint16_t g_read_sync_buf_max;

int STS3215_ReadByte1(uint8_t idx, uint8_t mem_addr) {
    uint8_t dat;
    int size = STS3215_ReadNormal(idx, mem_addr, &dat, 1);
    if (size != 1) {
        return -1;
    }
    else {
        return dat;
    }
}

int STS3215_ReadByte2(uint8_t idx, uint8_t mem_addr) {
    uint8_t dat[2];
    int size;
    uint16_t dat_word;
    size = STS3215_ReadNormal(idx, mem_addr, dat, 2);
    if (size != 2) {
        return -1;
    }
    dat_word = STS3215_CvrtByteToWord(dat[0], dat[1]);
    return dat_word;
}

int STS3215_ReadNormal(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len) {
    int size;
    uint8_t buf[4];
    uint8_t sum;
    uint8_t i;

    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, mem_addr, &dat_len, 1, STS3215_INST_READ);
    STS3215_FlushSerialTranBuf();

    g_status_comm = 0;

    if (!STS3215_CheckHead()) {
        g_status_comm = STS3215_ERR_NO_REPLY;
        return 0;
    }
    if (STS3215_ReadSerial(buf, 3) != 3) {
        g_status_comm = STS3215_ERR_NO_REPLY;
        return 0;
    }
    if (buf[0] != idx && idx != 0xfe) {
        g_status_comm = STS3215_ERR_SLAVE_ID;
        return 0;
    }
    if (buf[1] != (dat_len + 2)) {
        g_status_comm = STS3215_ERR_BUFF_LEN;
        return 0;
    }

    size = STS3215_ReadSerial(dat, dat_len);

    if (size != dat_len){
        g_status_comm = STS3215_ERR_NO_REPLY;
        return 0;
    }
    if (STS3215_ReadSerial(buf + 3, 1) != 1){
        g_status_comm = STS3215_ERR_NO_REPLY;
        return 0;
    }

    sum = buf[0] + buf[1] + buf[2];

    for (i = 0; i < size; i++) {
        sum += dat[i];
    }

    sum = ~sum;

    if (sum != buf[3]) {
        g_status_comm = STS3215_ERR_CRC_CMP;
        return 0;
    }

    g_status_servo = buf[2];

    return size;
}

void STS3215_ReadSyncBegin(uint8_t id_len, uint8_t rx_len) {
    g_read_sync_buf_max = id_len * (rx_len + 6);
    g_read_sync_buf = malloc(g_read_sync_buf_max);
}

void STS3215_ReadSyncEnd(void) {
    if (g_read_sync_buf) {
        free(g_read_sync_buf);
        g_read_sync_buf = NULL;
    }
}

int STS3215_WriteAsync(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len) {
    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, mem_addr, dat, dat_len, STS3215_INST_REG_WRITE);
    STS3215_FlushSerialTranBuf();
    return STS3215_Ack(idx);
}

int STS3215_WriteAsyncAction(uint8_t idx) {
    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, 0, NULL, 0, STS3215_INST_REG_ACTION);
    STS3215_FlushSerialTranBuf();
    return STS3215_Ack(idx);
}


int STS3215_writeByte1(uint8_t idx, uint8_t mem_addr, uint8_t dat_byt_1) {
    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, mem_addr, &dat_byt_1, 1, STS3215_INST_WRITE);
    STS3215_FlushSerialTranBuf();
    return STS3215_Ack(idx);
}

int STS3215_WriteByte2(uint8_t idx, uint8_t mem_addr, uint16_t dat_byt_2) {
    uint8_t buf[2];
    STS3215_CvrtWordToByte(buf + 0, buf + 1, dat_byt_2);
    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, mem_addr, buf, 2, STS3215_INST_WRITE);
    STS3215_FlushSerialTranBuf();
    return STS3215_Ack(idx);
}

int STS3215_WriteNormal(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len) {
    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, mem_addr, dat, dat_len, STS3215_INST_WRITE);
    STS3215_FlushSerialTranBuf();
    return STS3215_Ack(idx);
}

void STS3215_WriteSync(uint8_t idx[], uint8_t id_len, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len) {
    uint8_t len = ((dat_len + 1) * id_len + 4);
    uint8_t sum = 0;
    uint8_t buf[7];
    uint8_t i, j;

    buf[0] = 0xff;
    buf[1] = 0xff;
    buf[2] = 0xfe;
    buf[3] = len;
    buf[4] = STS3215_INST_SYNC_WRITE;
    buf[5] = mem_addr;
    buf[6] = dat_len;

    STS3215_FlushSerialRecvBuf();
    STS3215_WriteSerial(buf, 7);

    sum = 0xfe + len + STS3215_INST_SYNC_WRITE + mem_addr + dat_len;

    for (i  =0; i < id_len; i++) {
        STS3215_WriteSerial(&idx[i], 1);
        STS3215_WriteSerial(dat + i * dat_len, dat_len);
        sum += idx[i];
        for (j = 0; j < dat_len; j++) {
            sum += dat[i * dat_len + j];
        }
    }

    sum = ~sum;

    STS3215_WriteSerial(&sum, 1);
    STS3215_FlushSerialTranBuf();
}

int STS3215_DecoReadPacketByte1(void) {
    if (g_read_sync_packet_idx>=g_read_sync_packet_len) {
        return -1;
    }
    return g_read_sync_packet[g_read_sync_packet_idx++];
}

int STS3215_DecoReadPacketByte2(uint8_t neg_bit) {
    if ((g_read_sync_packet_idx+1) >= g_read_sync_packet_len) {
        return -1;
    }
    int word = STS3215_CvrtByteToWord(g_read_sync_packet[g_read_sync_packet_idx],
                                      g_read_sync_packet[g_read_sync_packet_idx + 1]);
    g_read_sync_packet_idx += 2;
    if (neg_bit) {
        if (word & (1 << neg_bit)) {
            word = -(word & ~(1 << neg_bit));
        }
    }
    return word;
}

int STS3215_Ping(uint8_t idx) {
    uint8_t buf[4];
    uint8_t sum;

    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, 0, NULL, 0, STS3215_INST_PING);
    STS3215_FlushSerialTranBuf();
    g_status_servo = 0;

    if (!STS3215_CheckHead()) {
        g_status_comm = STS3215_ERR_NO_REPLY;
        return -1;
    }

    g_status_comm = 0;

    if (STS3215_ReadSerial(buf, 4) != 4) {
        g_status_comm = STS3215_ERR_NO_REPLY;
        return -1;
    }
    if (buf[0] != idx && idx != 0xfe) {
        g_status_comm = STS3215_ERR_SLAVE_ID;
        return -1;
    }
    if (buf[1] != 2){
        g_status_comm = STS3215_ERR_BUFF_LEN;
        return -1;
    }

    sum = ~(buf[0] + buf[1] + buf[2]);

    if (sum != buf[3]) {
        g_status_comm = STS3215_ERR_CRC_CMP;
        return -1;
    }

    g_status_servo = buf[2];

    return buf[0];
}

int STS3215_Reset(uint8_t idx) {
    uint8_t buf[4];
    uint8_t sum;

    STS3215_FlushSerialRecvBuf();
    STS3215_WriteBuf(idx, 0, NULL, 0, STS3215_INST_RESET);
    STS3215_FlushSerialTranBuf();
    g_status_servo = 0;

    if (!STS3215_CheckHead()) {
        g_status_comm = STS3215_ERR_NO_REPLY;
        return -1;
    }

    g_status_comm = 0;

    if (STS3215_ReadSerial(buf, 4) != 4) {
        g_status_comm = STS3215_ERR_NO_REPLY;
        return -1;
    }
    if (buf[0] != idx && idx != 0xfe) {
        g_status_comm = STS3215_ERR_SLAVE_ID;
        return -1;
    }
    if (buf[1] != 2) {
        g_status_comm = STS3215_ERR_BUFF_LEN;
        return -1;
    }

    sum = ~(buf[0] + buf[1] + buf[2]);

    if (sum != buf[3]) {
        g_status_comm = STS3215_ERR_CRC_CMP;
        return -1;
    }

    g_status_servo = buf[2];

    return buf[0];
}

int STS3215_RecvReadSyncPacket(uint8_t idx, uint8_t *dat) {
    uint16_t read_sync_buf_idx = 0;
    g_read_sync_packet = dat;
    g_read_sync_packet_idx = 0;
    g_status_servo = 0;

    while ((read_sync_buf_idx + 6 + g_read_sync_packet_len) <= g_read_sync_buf_len) {
        uint8_t buf[] = {0, 0, 0};
        uint8_t sum = 0;
        while (read_sync_buf_idx < g_read_sync_buf_len){
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = g_read_sync_buf[read_sync_buf_idx++];
            if(buf[0] == 0xff && buf[1] == 0xff && buf[2] != 0xff) {
                g_status_comm = STS3215_ERR_NO_REPLY;
                break;
            }
        }
        if (buf[2] != idx) {
            g_status_comm = STS3215_ERR_SLAVE_ID;
            continue;
        }
        if (g_read_sync_buf[read_sync_buf_idx++] != (g_read_sync_packet_len + 2)) {
            continue;
        }
        g_status_servo = g_read_sync_buf[read_sync_buf_idx++];
        sum = idx + (g_read_sync_packet_len + 2) + g_status_servo;
        for (uint8_t i = 0; i < g_read_sync_packet_len; i++) {
            g_read_sync_packet[i] = g_read_sync_buf[read_sync_buf_idx++];
            sum += g_read_sync_packet[i];
        }
        sum = ~sum;
        if (sum != g_read_sync_buf[read_sync_buf_idx++]) {
            g_status_comm = STS3215_ERR_CRC_CMP;
            return 0;
        }
        return g_read_sync_packet_len;
    }

    return 0;
}

int STS3215_TranReadSyncPacket(uint8_t idx[], uint8_t id_len, uint8_t mem_addr, uint8_t dat_len) {
    uint8_t sum;
    uint8_t i;

    STS3215_FlushSerialRecvBuf();
    g_read_sync_packet_len = dat_len;
    sum = (4 + 0xfe) + id_len + mem_addr + dat_len + STS3215_INST_SYNC_READ;

    STS3215_WriteSerialByte(0xff);
    STS3215_WriteSerialByte(0xff);
    STS3215_WriteSerialByte(0xfe);
    STS3215_WriteSerialByte(id_len + 4);
    STS3215_WriteSerialByte(STS3215_INST_SYNC_READ);
    STS3215_WriteSerialByte(mem_addr);
    STS3215_WriteSerialByte(dat_len);

    for (i = 0; i < id_len; i++) {
        STS3215_WriteSerialByte(idx[i]);
        sum += idx[i];
    }

    sum = ~sum;

    STS3215_WriteSerialByte(sum);
    STS3215_FlushSerialTranBuf();

    g_read_sync_buf_len = STS3215_ReadSerial(g_read_sync_buf, g_read_sync_buf_max);

    return g_read_sync_buf_len;
}

int STS3215_Ack(uint8_t idx) {
    uint8_t buf[4];
    uint8_t sum;
    g_status_comm = 0;

    if (idx != 0xfe && g_level) {
        if (!STS3215_CheckHead()) {
            g_status_comm = STS3215_ERR_NO_REPLY;
            return 0;
        }
        g_status_servo = 0;
        if (STS3215_ReadSerial(buf, 4) != 4) {
            g_status_comm = STS3215_ERR_NO_REPLY;
            return 0;
        }
        if (buf[0] != idx) {
            g_status_comm = STS3215_ERR_SLAVE_ID;
            return 0;
        }
        if (buf[1]!=2) {
            g_status_comm = STS3215_ERR_BUFF_LEN;
            return 0;
        }
        sum = ~(buf[0] + buf[1] + buf[2]);
        if (sum != buf[3]) {
            g_status_comm = STS3215_ERR_CRC_CMP;
            return 0;
        }
        g_status_servo = buf[2];
    }

    return 1;
}

int STS3215_CheckHead(void) {
    uint8_t dat;
    uint8_t buf[2] = {0, 0};
    uint8_t cnt = 0;

    while (1) {
        if (!STS3215_ReadSerial(&dat, 1)) {
            return 0;
        }
        buf[1] = buf[0];
        buf[0] = dat;
        if (buf[0] == 0xff && buf[1] == 0xff) {
            break;
        }
        cnt++;
        if (cnt > 10) {
            return 0;
        }
    }

    return 1;
}

int STS3215_CvrtByteToWord(uint8_t dat_l, uint8_t dat_h) {
    int dat;

    if (g_end) {
        dat = dat_l;
        dat <<=8;
        dat |= dat_h;
    }
    else {
        dat = dat_h;
        dat <<=8;
        dat |= dat_l;
    }

    return dat;
}

void STS3215_CvrtWordToByte(uint8_t *dat_l, uint8_t* dat_h, int dat) {
    if (g_end) {
        *dat_l = (dat >> 8);
        *dat_h = (dat & 0xff);
    }
    else {
        *dat_h = (dat >> 8);
        *dat_l = (dat & 0xff);
    }
}

void STS3215_WriteBuf(uint8_t idx, uint8_t mem_addr, uint8_t *dat, uint8_t dat_len, uint8_t fun) {
    uint8_t i;
    uint8_t len = 2;
    uint8_t buf[6];
    uint8_t sum = 0;
    buf[0] = 0xff;
    buf[1] = 0xff;
    buf[2] = idx;
    buf[4] = fun;

    if (dat) {
        len += dat_len + 1;
        buf[3] = len;
        buf[5] = mem_addr;
        STS3215_WriteSerial(buf, 6);
    }
    else {
        buf[3] = len;
        STS3215_WriteSerial(buf, 5);
    }

    sum = idx + len + fun + mem_addr;

    if (dat) {
        for (i = 0; i < dat_len; i++){
            sum += dat[i];
        }
        STS3215_WriteSerial(dat, dat_len);
    }

    sum = ~sum;

    STS3215_WriteSerial(&sum, 1);
}

uint8_t STS3215_GetEndian(void) {
    return g_end;
}

int STS3215_GetStatusComm(void) {
    return g_status_comm;
}

int STS3215_GetStatusServo(void) {
    return g_status_servo;
}

void STS3215_SetEndian(uint8_t end) {
    g_end = end;
}

void STS3215_SetLevel(uint8_t lev) {
    g_level = lev;
}
