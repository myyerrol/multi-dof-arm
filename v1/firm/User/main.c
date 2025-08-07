#include <debug.h>
#include <ps2_joystick.h>
#include <sts3215_comm.h>
#include <sts3215.h>

#define TEST TEST_PS2_JOYSTICK

#define TEST_PS2_JOYSTICK     0
#define TEST_MOVE_DEFAULT_POS 1
#define TEST_PICK_AND_PLACE   2

#ifndef TEST
    #define TEST TEST_MOVE_DEFAULT_POS
#endif

void MAIN_InitSys(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    Delay_Ms(2000);
}

void MAIN_InitDrv(void) {
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate             = 115200;
    USART_InitStructure.USART_WordLength           = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits             = USART_StopBits_1;
    USART_InitStructure.USART_Parity               = USART_Parity_No;
    USART_InitStructure.USART_Mode                 = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void MAIN_InitMod(void) {
    PS2_JOYSTICK_Init();
}

uint8_t  g_idx[6] = {1, 2, 3, 4, 5, 6};
int16_t  g_pos[6];
uint16_t g_spd[6] = {1000};
uint8_t  g_acc[6] = {50};

void MAIN_SetupTest(void) {
    STS3215_SetEndian(0);
}

void MAIN_TestJoystick(void) {
    PS2_JOYSTICK_Test();
    Delay_Ms(50);
}

void MAIN_TestMoveDefaultPos(void) {
    g_pos[0] = 2048;
    g_pos[1] = 2048;
    g_pos[2] = 2048;
    g_pos[3] = 2048;
    g_pos[4] = 2048;
    g_pos[5] = 2700;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[0] = 2048;
    g_pos[1] = 2048;
    g_pos[2] = 2048;
    g_pos[3] = 3072;
    g_pos[4] = 2048;
    g_pos[5] = 2700;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[0] = 2048;
    g_pos[1] = 1024;
    g_pos[2] = 2048;
    g_pos[3] = 3072;
    g_pos[4] = 2048;
    g_pos[5] = 2700;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[5] = 3900;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[5] = 2700;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);
}

void MAIN_TestPickAndPlace(void) {
    g_pos[0] = 1400;
    g_pos[1] = 2522;
    g_pos[2] = 2315;
    g_pos[3] = 1355;
    g_pos[4] = 2048;
    g_pos[5] = 3900;
    g_spd[0] = 300;
    g_spd[1] = 300;
    g_spd[2] = 300;
    g_spd[3] = 300;
    g_spd[4] = 300;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(5000);

    g_pos[5] = 3075;
    g_spd[0] = 1000;
    g_spd[1] = 1000;
    g_spd[2] = 1000;
    g_spd[3] = 1000;
    g_spd[4] = 1000;
    g_spd[5] = 1000;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[3] = 1000;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[1] = 1113;
    g_pos[2] = 2807;
    g_pos[3] = 1000;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[0] = 2676;
    g_pos[3] = 1000;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[1] = 2468;
    g_pos[2] = 2538;
    g_pos[3] = 1156;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[5] = 3900;
    g_spd[5] = 200;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(3000);

    g_pos[3] = 500;
    g_spd[5] = 1000;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);

    g_pos[0] = 2048;
    g_pos[1] = 2048;
    g_pos[2] = 2048;
    g_pos[4] = 2048;
    g_pos[5] = 2700;
    g_spd[0] = 400;
    g_spd[1] = 400;
    g_spd[2] = 400;
    g_spd[3] = 400;
    g_spd[4] = 400;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(4000);

    g_pos[5] = 3900;
    STS3215_WriteSyncPos(g_idx, 6, g_pos, g_spd, g_acc);
    Delay_Ms(2000);
}

void MAIN_TestJoystickControlFK(void) {
    PS2_JOYSTICK_InitTypeDef PS2_JOYSTICK_InitStructure = {0};
    PS2_JOYSTICK_ScanKey(&PS2_JOYSTICK_InitStructure);

    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x10) {
        g_pos[0] += 1;
        printf("0+\n");
    }
    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x40) {
        g_pos[0] -= 1;
        printf("0-\n");
    }

    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x80) {
        g_pos[1] += 1;
        printf("1+\n");
    }
    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x20) {
        g_pos[1] -= 1;
        printf("1-\n");
    }

    if (PS2_JOYSTICK_InitStructure.btn_2 == 0x10) {
        g_pos[2] += 1;
        printf("2+\n");
    }
    if (PS2_JOYSTICK_InitStructure.btn_2 == 0x40) {
        g_pos[2] -= 1;
        printf("2-\n");
    }

    if (PS2_JOYSTICK_InitStructure.btn_2 == 0x80) {
        g_pos[3] += 1;
        printf("3+\n");
    }
    if (PS2_JOYSTICK_InitStructure.btn_2 == 0x20) {
        g_pos[3] -= 1;
        printf("3-\n");
    }

    if (PS2_JOYSTICK_InitStructure.btn_2 == 0x04) {
        g_pos[4] += 1;
        printf("4+\n");
    }
    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x01) {
        g_pos[4] -= 1;
        printf("4-\n");
    }

    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x08) {
        g_pos[5] += 1;
        printf("5+\n");
    }
    if (PS2_JOYSTICK_InitStructure.btn_1 == 0x02) {
        g_pos[5] -= 1;
        printf("5-\n");
    }
}

int main(void) {
    MAIN_InitSys();
    MAIN_InitDrv();
    MAIN_InitMod();

    MAIN_SetupTest();

    while (1) {
#if (TEST == TEST_PS2_JOYSTICK)
        MAIN_TestJoystick();
#elif (TEST == TEST_MOVE_DEFAULT_POS)
        MAIN_TestMoveDefaultPos();
#elif (TEST == TEST_PICK_AND_PLACE)
        MAIN_TestPickAndPlace();
#endif
    }
}
