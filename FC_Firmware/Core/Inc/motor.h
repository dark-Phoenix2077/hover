#ifndef MOTOR_H
#define MOTOR_H
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define MOTOR_FL 0
#define MOTOR_FR 1
#define MOTOR_RL 2
#define MOTOR_RR 3
#define MOTOR_MAX    1000
#define MOTOR_ARM_SPIN 50

void  Motor_Init(TIM_HandleTypeDef *htim);
void  Motor_SetAll(uint16_t m0,uint16_t m1,uint16_t m2,uint16_t m3);
void  Motor_Arm(void);
void  Motor_Disarm(void);
bool  Motor_IsArmed(void);
void  Motor_Mix(uint16_t throttle, int16_t roll, int16_t pitch, int16_t yaw);
#endif
