#ifndef PS2_JOYSTICK_H
#define PS2_JOYSTICK_H

#include <stdint.h>

#define PS2_JOYSTICK_PIN_DAT GPIO_Pin_1
#define PS2_JOYSTICK_PIN_CMD GPIO_Pin_2
#define PS2_JOYSTICK_PIN_CS  GPIO_Pin_3
#define PS2_JOYSTICK_PIN_CLK GPIO_Pin_4

#define DI()     GPIO_ReadInputDataBit(GPIOC, PS2_JOYSTICK_PIN_DAT)
#define CMD_H()  GPIO_WriteBit(GPIOC, PS2_JOYSTICK_PIN_CMD, 1)
#define CMD_L()  GPIO_WriteBit(GPIOC, PS2_JOYSTICK_PIN_CMD, 0)
#define CS_H()   GPIO_WriteBit(GPIOC, PS2_JOYSTICK_PIN_CS,  1)
#define CS_L()   GPIO_WriteBit(GPIOC, PS2_JOYSTICK_PIN_CS,  0)
#define CLK_H()  GPIO_WriteBit(GPIOC, PS2_JOYSTICK_PIN_CLK, 1)
#define CLK_L()  GPIO_WriteBit(GPIOC, PS2_JOYSTICK_PIN_CLK, 0)

typedef struct {
    uint8_t mode;
    uint8_t btn_1;
    uint8_t btn_2;
    uint8_t joy_r_lf_rt;
    uint8_t joy_r_up_dw;
    uint8_t joy_l_lf_rt;
    uint8_t joy_l_up_dw;
} PS2_JOYSTICK_InitTypeDef;

void PS2_JOYSTICK_Init(void);
void PS2_JOYSTICK_ScanKey(PS2_JOYSTICK_InitTypeDef *PS2_JOYSTICK_InitStruct);
void PS2_JOYSTICK_Test(void);

#endif
