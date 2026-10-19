//
// Created by 千枫眠 on 2026/10/18.
//

#ifndef LEARN_SERVO_H
#define LEARN_SERVO_H
#include <stdint.h>


#define SERVO_MIN_US 1000U
#define SERVO_MID_US 1500U
#define SERVO_MAX_US 2000U

void Servo_Init(void);
void Servo_Set(uint8_t id,uint16_t pulse_us);
void Servo_SetAngle(uint8_t id,uint8_t angle);
#endif //LEARN_SERVO_H
