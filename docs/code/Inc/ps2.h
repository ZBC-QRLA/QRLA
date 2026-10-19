//
// Created by 千枫眠 on 2026/10/18.
//

#ifndef LEARN_PS2_H
#define LEARN_PS2_H

#include <stdint.h>

void    PS2_Init(void);
uint8_t PS2_ReadPacket(uint8_t *buf);   // 读 9 字节；返回 1 = 第 3 字节是 0x5A（数据有效）

typedef struct
{
    uint8_t mode;                 // 0x41 = 数字模式 / 0x73 = 模拟模式
    uint8_t btn1;                 // 已取反：按下 = 1
    uint8_t btn2;                 // 已取反：按下 = 1
    uint8_t RJoy_LR, RJoy_UD;     // 右摇杆：0=左/上，0x80≈中位，0xFF=右/下
    uint8_t LJoy_LR, LJoy_UD;     // 左摇杆：同上
} PS2_TypeDef;

uint8_t PS2_Scan(PS2_TypeDef *pad);                 // 读一包并填结构体；返回 1 = 成功
void    PS2_PrintKeys(const PS2_TypeDef *pad);      // 调试用：把按下的键名打到串口
#endif //LEARN_PS2_H
