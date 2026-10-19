/* ============================================================================
*  motor.c  ——  电机控制模块 · 实现
 *
 *  硬件对应关系：
 *      电机1：PWM = PA6 (TIM3_CH1)    方向 = PB12
 *      电机2：PWM = PA7 (TIM3_CH2)    方向 = PB13
 *      电机3：PWM = PB0 (TIM3_CH3)    方向 = PB14
 *      电机4：PWM = PB1 (TIM3_CH4)    方向 = PB15
 * ==========================================================================*/
//
#include"main.h"
#include"motor.h"

static GPIO_TypeDef * const DIR_PORT[4]={GPIOB,GPIOB,GPIOB,GPIOB};
static const uint16_t DIR_PIN[4]={GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
static const uint32_t CH[4]={TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_3,TIM_CHANNEL_4};

extern TIM_HandleTypeDef htim3;

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

    Motor_Set(0,0);
    Motor_Set(1,0);
    Motor_Set(2,0);
    Motor_Set(3,0);
}

void Motor_Set(uint8_t id,int32_t speed)
{
    if (speed>=100) speed=100;
    else if (speed<=-100) speed=-100;
    if (speed<0)
    {
        HAL_GPIO_WritePin(DIR_PORT[id],DIR_PIN[id],GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3,CH[id],-speed);
    }
    else
    {
        HAL_GPIO_WritePin(DIR_PORT[id],DIR_PIN[id],GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3,CH[id],speed);
    }
}

uint8_t Motor_GetDir(uint8_t id)
{
    if (HAL_GPIO_ReadPin(DIR_PORT[id],DIR_PIN[id])==GPIO_PIN_RESET) return 0;
    else return 1;
}
// Created by 千枫眠 on 2026/10/11.
//
