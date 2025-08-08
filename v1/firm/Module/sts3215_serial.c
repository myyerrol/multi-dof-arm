#include <debug.h>
#include <sts3215_serial.h>

uint8_t g_buf[128];
uint8_t g_buf_len = 0;
int     g_timeout = 5000;

void STS3215_FlushSerialRecvBuf() {
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET);
}

void STS3215_FlushSerialTranBuf() {
    if (g_buf_len) {
        for (uint8_t i = 0; i < g_buf_len; i++) {
            USART_SendData(USART1, g_buf[i]);
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        }
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        g_buf_len = 0;
    }
}

int STS3215_ReadSerial(uint8_t *dat, int dat_len) {
    int size = 0;
    int time = 0;
    int dat_recv;

    while (1) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
        dat_recv = USART_ReceiveData(USART1);
        if (dat_recv != -1) {
            if (dat) {
                dat[size] = dat_recv;
            }
            size++;
            time = 0;
        }
        if (size >= dat_len) {
            break;
        }
        time++;
        if (time > g_timeout) {
            break;
        }
    }

    return size;
}

int STS3215_WriteSerial(uint8_t *dat, int dat_len) {
    while (dat_len--) {
        if (g_buf_len<sizeof(g_buf)) {
            g_buf[g_buf_len] = *dat;
            g_buf_len++;
            dat++;
        }
    }
    return g_buf_len;
}

int STS3215_WriteSerialByte(uint8_t dat_byt) {
    if(g_buf_len < sizeof(g_buf)){
        g_buf[g_buf_len] = dat_byt;
        g_buf_len++;
    }
    return g_buf_len;
}
