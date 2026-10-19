//
// Created by 千枫眠 on 2026/10/18.
//
#include "main.h"
#include "ps2.h"
#include<stdio.h>
#define PS2_CS_L()   HAL_GPIO_WritePin(PS2_CS_GPIO_Port,  PS2_CS_Pin,  GPIO_PIN_RESET)
#define PS2_CS_H()   HAL_GPIO_WritePin(PS2_CS_GPIO_Port,  PS2_CS_Pin,  GPIO_PIN_SET)
#define PS2_CLK_L()  HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_RESET)
#define PS2_CLK_H()  HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET)
#define PS2_CMD_L()  HAL_GPIO_WritePin(PS2_CMD_GPIO_Port, PS2_CMD_Pin, GPIO_PIN_RESET)
#define PS2_CMD_H()  HAL_GPIO_WritePin(PS2_CMD_GPIO_Port, PS2_CMD_Pin, GPIO_PIN_SET)
#define PS2_DAT_IS_HIGH()  (HAL_GPIO_ReadPin(PS2_DAT_GPIO_Port, PS2_DAT_Pin) == GPIO_PIN_SET)

static const uint8_t PS2_REQ[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static void PS2_DelayUs(uint32_t us)
{
    uint32_t start  = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000U);   // 1µs = 72 个周期
    while ((DWT->CYCCNT - start) < cycles) { }
}

void PS2_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;   // 打开 DWT 总开关
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;             // 让 CYCCNT 每 1 个 CPU 时钟 +1

    PS2_CS_H();
    PS2_CLK_H();
    PS2_CMD_H();
    PS2_DelayUs(200);                                 // 等接收器上电稳定
}

static uint8_t PS2_Xfer(uint8_t cmd)
{
    uint8_t res = 0;

    for (uint8_t mask = 0x01; mask != 0; mask <<= 1)
    {
        if (cmd & mask) PS2_CMD_H(); else PS2_CMD_L();  // ① 先摆好这一位（LSB first）

        PS2_CLK_L();                                    // ② 时钟下降沿：手柄借这个沿把数据推上来
        PS2_DelayUs(16);                                // ③ 等线上稳
        if (PS2_DAT_IS_HIGH()) res |= mask;             // ④ 在低电平期间读回来
        PS2_CLK_H();                                    // ⑤ 上升沿
        PS2_DelayUs(16);                                // ⑥ 再等半周期
    }

    return res;
}

uint8_t PS2_ReadPacket(uint8_t *buf)
{
    PS2_CS_L();                                   // 片选拉低 = “开始通信”
    for (uint8_t i = 0; i < 9; i++) buf[i] = PS2_Xfer(PS2_REQ[i]);
    PS2_CS_H();                                   // 片选拉高 = “结束”

    return (buf[2] == 0x5A) ? 1 : 0;
}
uint8_t PS2_Scan(PS2_TypeDef *pad)
{
    uint8_t pkt[9];
    if (!PS2_ReadPacket(pkt)) return 0;      // 没读到 0x5A 就保留上一包的值

    pad->mode    = pkt[1];
    pad->btn1    = (uint8_t)~pkt[3];         // 取反：手柄是“按下=0”，我们要“按下=1”
    pad->btn2    = (uint8_t)~pkt[4];
    pad->RJoy_LR = pkt[5];
    pad->RJoy_UD = pkt[6];
    pad->LJoy_LR = pkt[7];
    pad->LJoy_UD = pkt[8];
    return 1;
}

void PS2_PrintKeys(const PS2_TypeDef *pad)
{
    static const char * const N1[8] = {"SEL", "J-R", "J-L", "STR", "UP", "RGT", "DWN", "LFT"};
    static const char * const N2[8] = {"L2", "R2", "L1", "R1", "TRI", "CIR", "CRO", "SQU"};

    printf("mode=%02X keys:", pad->mode);
    for (uint8_t i = 0; i < 8; i++) if (pad->btn1 & (1U << i)) printf(" %s", N1[i]);
    for (uint8_t i = 0; i < 8; i++) if (pad->btn2 & (1U << i)) printf(" %s", N2[i]);
    if (!pad->btn1 && !pad->btn2) printf(" (none)");
    printf("\r\n");
}