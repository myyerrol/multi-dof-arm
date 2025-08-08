#include <sts3215_comm.h>
#include <sts3215_inst.h>
#include <sts3215.h>

int STS3215_WriteAsyncPos(uint8_t idx, int16_t pos, uint16_t spd, uint8_t acc) {
    uint8_t buf[7];

    if (pos < 0) {
        pos = -pos;
        pos |= (1 << 15);
    }

    buf[0] = acc;

    STS3215_CvrtWordToByte(buf + 1, buf + 2, pos);
    STS3215_CvrtWordToByte(buf + 3, buf + 4, 0);
    STS3215_CvrtWordToByte(buf + 5, buf + 6, spd);

    return STS3215_WriteAsync(idx, STS3215_SRAM_RW_ACC, buf, 7);
}

int STS3215_WriteNormalPos(uint8_t idx, int16_t pos, uint16_t spd, uint8_t acc) {
    uint8_t buf[7];

    if (pos < 0) {
        pos = -pos;
        pos |= (1 << 15);
    }

    buf[0] = acc;

    STS3215_CvrtWordToByte(buf + 1, buf + 2, pos);
    STS3215_CvrtWordToByte(buf + 3, buf + 4, 0);
    STS3215_CvrtWordToByte(buf + 5, buf + 6, spd);

    return STS3215_WriteNormal(idx, STS3215_SRAM_RW_ACC, buf, 7);
}

void STS3215_WriteSyncPos(uint8_t idx[], uint8_t id_len, int16_t pos[], uint16_t spd[], uint8_t acc[]) {
    uint8_t buf[32 * 7];
    uint16_t spd_tmp;

    for (uint8_t i = 0; i < id_len; i++) {
        if (pos[i] < 0) {
            pos[i] = -pos[i];
            pos[i] |= (1 << 15);
        }

        if (spd) {
            spd_tmp = spd[i];
        }
        else {
            spd_tmp = 0;
        }

        if (acc) {
            buf[i * 7] = acc[i];
        }
        else{
            buf[i * 7] = 0;
        }

        STS3215_CvrtWordToByte(buf + i * 7 + 1, buf + i * 7 + 2, pos[i]);
        STS3215_CvrtWordToByte(buf + i * 7 + 3, buf + i * 7 + 4, 0);
        STS3215_CvrtWordToByte(buf + i * 7 + 5, buf + i * 7 + 6, spd_tmp);
    }

    STS3215_WriteSync(idx, id_len, STS3215_SRAM_RW_ACC, buf, 7);
}

int STS3215_WriteWheelSpeed(uint8_t idx, int16_t spd, uint8_t acc) {
    uint8_t buf[2];

    if (spd < 0) {
        spd = -spd;
        spd |= (1 << 15);
    }

    buf[0] = acc;

    STS3215_WriteNormal(idx, STS3215_SRAM_RW_ACC, buf, 1);
    STS3215_CvrtWordToByte(buf + 0, buf + 1, spd);
    STS3215_WriteNormal(idx, STS3215_SRAM_RW_GOAL_SPEED_L, buf, 2);

    return 1;
}

int STS3215_SetEPROMLock(uint8_t idx) {
    return STS3215_writeByte1(idx, STS3215_SRAM_RW_LOCK, 1);
}

int STS3215_SetEPROMUnlock(uint8_t idx) {
    return STS3215_writeByte1(idx, STS3215_SRAM_RW_LOCK, 0);
}

int STS3215_SetMiddPosCalib(uint8_t idx) {
    return STS3215_writeByte1(idx, STS3215_SRAM_RW_TORQUE_ENABLE, 128);
}

int STS3215_SetWheelSpeed(uint8_t idx) {
    return STS3215_writeByte1(idx, STS3215_EPROM_RW_MODE, 1);
}
