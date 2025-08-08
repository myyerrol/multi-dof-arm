#ifndef STS3215_H
#define STS3215_H

#include <stdint.h>

#define STS3215_RATE_1M                     0
#define STS3215_RATE_0_5M                   1
#define STS3215_RATE_250K                   2
#define STS3215_RATE_128K                   3
#define STS3215_RATE_115200                 4
#define STS3215_RATE_76800                  5
#define STS3215_RATE_57600                  6
#define STS3215_RATE_38400                  7

#define STS3215_EPROM_RO_MODEL_L            3
#define STS3215_EPROM_RO_MODEL_H            4

#define STS3215_EPROM_RW_ID                 5
#define STS3215_EPROM_RW_BAUD_RATE          6
#define STS3215_EPROM_RW_MIN_ANGLE_LIMIT_L  9
#define STS3215_EPROM_RW_MIN_ANGLE_LIMIT_H  10
#define STS3215_EPROM_RW_MAX_ANGLE_LIMIT_L  11
#define STS3215_EPROM_RW_MAX_ANGLE_LIMIT_H  12
#define STS3215_EPROM_RW_CW_DEAD            26
#define STS3215_EPROM_RW_CCW_DEAD           27
#define STS3215_EPROM_RW_OFS_L              31
#define STS3215_EPROM_RW_OFS_H              32
#define STS3215_EPROM_RW_MODE               33

#define STS3215_SRAM_RW_TORQUE_ENABLE       40
#define STS3215_SRAM_RW_ACC                 41
#define STS3215_SRAM_RW_GOAL_POSITION_L     42
#define STS3215_SRAM_RW_GOAL_POSITION_H     43
#define STS3215_SRAM_RW_GOAL_TIME_L         44
#define STS3215_SRAM_RW_GOAL_TIME_H         45
#define STS3215_SRAM_RW_GOAL_SPEED_L        46
#define STS3215_SRAM_RW_GOAL_SPEED_H        47
#define STS3215_SRAM_RW_LOCK                55

#define STS3215_SRAM_RO_PRESENT_POSITION_L  56
#define STS3215_SRAM_RO_PRESENT_POSITION_H  57
#define STS3215_SRAM_RO_PRESENT_SPEED_L     58
#define STS3215_SRAM_RO_PRESENT_SPEED_H     59
#define STS3215_SRAM_RO_PRESENT_LOAD_L      60
#define STS3215_SRAM_RO_PRESENT_LOAD_H      61
#define STS3215_SRAM_RO_PRESENT_VOLTAGE     62
#define STS3215_SRAM_RO_PRESENT_TEMPERATURE 63
#define STS3215_SRAM_RO_MOVING              66
#define STS3215_SRAM_RO_PRESENT_CURRENT_L   69
#define STS3215_SRAM_RO_PRESENT_CURRENT_H   70

int  STS3215_WriteAsyncPos(uint8_t idx, int16_t pos, uint16_t spd, uint8_t acc);
int  STS3215_WriteNormalPos(uint8_t idx, int16_t pos, uint16_t spd, uint8_t acc);
void STS3215_WriteSyncPos(uint8_t idx[], uint8_t id_len, int16_t pos[], uint16_t spd[], uint8_t acc[]);
int  STS3215_WriteWheelSpeed(uint8_t idx, int16_t spd, uint8_t acc);

int  STS3215_SetEPROMLock(uint8_t idx);
int  STS3215_SetEPROMUnlock(uint8_t idx);
int  STS3215_SetMiddPosCalib(uint8_t idx);
int  STS3215_SetWheelSpeed(uint8_t idx);

#endif
