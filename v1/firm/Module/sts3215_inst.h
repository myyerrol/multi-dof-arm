#ifndef STS3215_INST_H
#define STS3215_INST_H

#include <stdint.h>

#ifndef NULL
    #define NULL ((void *)0)
#endif

#define STS3215_INST_PING       0x01
#define STS3215_INST_READ       0x02
#define STS3215_INST_WRITE      0x03
#define STS3215_INST_REG_WRITE  0x04
#define STS3215_INST_REG_ACTION 0x05
#define STS3215_INST_SYNC_READ  0x82
#define STS3215_INST_SYNC_WRITE 0x83
#define STS3215_INST_RESET      0x0A
#define STS3215_INST_OFSCAL     0x0B

#endif
