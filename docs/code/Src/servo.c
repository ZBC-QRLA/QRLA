//
// Created by 千枫眠 on 2026/10/18.
//
/* 舵机1：PB6 (TIM4_CH1)
 * 舵机2：PB7 (TIM4_CH2)
 * 舵机3：PB8 (TIM4_CH3)
 */
#include"main.h"
#include"servo.h"

static const uint32_t SERVO_CH[3]={TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_3};
extern TIM_HandleTypeDef htim4;
void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim4,SERVO_CH[0]);
    HAL_TIM_PWM_Start(&htim4,SERVO_CH[1]);
    HAL_TIM_PWM_Start(&htim4,SERVO_CH[2]);
}

void Servo_Set(uint8_t id,uint16_t pulse_us)
{
    if (id>2) return;
    if (pulse_us<SERVO_MIN_US) pulse_us=SERVO_MIN_US;
    else if (pulse_us>SERVO_MAX_US) pulse_us=SERVO_MAX_US;

    __HAL_TIM_SET_COMPARE(&htim4,SERVO_CH[id],pulse_us);
}

void Servo_SetAngle(uint8_t id,uint8_t angle)
{
    if (angle>180) angle=180;
    uint32_t us=SERVO_MIN_US+(uint32_t)angle*(SERVO_MAX_US-SERVO_MIN_US)/180U;
    Servo_Set(id,(uint16_t)us);
}
