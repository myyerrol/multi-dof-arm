#include <debug.h>
#include <ps2_joystick.h>

const  uint8_t g_ps2_cmd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t g_ps2_dat[9] = {0};

static uint8_t PS2_JOYSTICK_ReadWriteData(uint8_t cmd) {
    volatile uint8_t res = 0;
    volatile uint8_t ref;

    for (ref = 0x01; ref > 0x00; ref <<= 1) {
        // Write data
        if (ref & cmd) {
            CMD_H();
        }
        else {
            CMD_L();
        }
        CLK_L();
        Delay_Us(16);

        // Read data
        if (DI()) {
            res |= ref;
        }
        CLK_H();
        Delay_Us(16);
    }

    return res;
}

void PS2_JOYSTICK_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin   = PS2_JOYSTICK_PIN_DAT;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = PS2_JOYSTICK_PIN_CMD;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = PS2_JOYSTICK_PIN_CS;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = PS2_JOYSTICK_PIN_CLK;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    CS_H();
}

void PS2_JOYSTICK_ScanKey(PS2_JOYSTICK_InitTypeDef *PS2_JOYSTICK_InitStruct) {
    CS_L();

    for (uint8_t i = 0; i < 9; i++) {
        g_ps2_dat[i] = PS2_JOYSTICK_ReadWriteData(g_ps2_cmd[i]);
    }

    CS_H();

    PS2_JOYSTICK_InitStruct->mode        =  g_ps2_dat[1];
    PS2_JOYSTICK_InitStruct->btn_1       = ~g_ps2_dat[3];
    PS2_JOYSTICK_InitStruct->btn_2       = ~g_ps2_dat[4];
    PS2_JOYSTICK_InitStruct->joy_r_lf_rt =  g_ps2_dat[5];
    PS2_JOYSTICK_InitStruct->joy_r_up_dw =  g_ps2_dat[6];
    PS2_JOYSTICK_InitStruct->joy_l_lf_rt =  g_ps2_dat[7];
    PS2_JOYSTICK_InitStruct->joy_l_up_dw =  g_ps2_dat[8];
}

void PS2_JOYSTICK_Test(void) {
    PS2_JOYSTICK_InitTypeDef PS2_JOYSTICK_InitStructure = {0};
    PS2_JOYSTICK_ScanKey(&PS2_JOYSTICK_InitStructure);

    printf("mode: %2x    "
           "btn_1: %2x    "
           "btn_2: %2x    "
           "joy_r_lf_rt: %2x    "
           "joy_r_up_dw: %2x    "
           "joy_l_lf_rt: %2x    "
           "joy_l_up_dw: %2x\n",
        PS2_JOYSTICK_InitStructure.mode,
        PS2_JOYSTICK_InitStructure.btn_1,
        PS2_JOYSTICK_InitStructure.btn_2,
        PS2_JOYSTICK_InitStructure.joy_r_lf_rt,
        PS2_JOYSTICK_InitStructure.joy_r_up_dw,
        PS2_JOYSTICK_InitStructure.joy_l_lf_rt,
        PS2_JOYSTICK_InitStructure.joy_l_up_dw);
}
