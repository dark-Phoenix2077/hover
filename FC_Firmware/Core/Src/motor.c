#include "motor.h"

static TIM_HandleTypeDef *_htim = NULL;
static bool _armed = false;

static uint32_t t2ccr(uint16_t t) {
    if(t>MOTOR_MAX) t=MOTOR_MAX;
    return (uint32_t)t*8999/MOTOR_MAX;
}

void Motor_Init(TIM_HandleTypeDef *htim) {
    _htim=htim;
    HAL_TIM_PWM_Start(htim,TIM_CHANNEL_1); HAL_TIM_PWM_Start(htim,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(htim,TIM_CHANNEL_3); HAL_TIM_PWM_Start(htim,TIM_CHANNEL_4);
    Motor_Disarm();
}

void Motor_SetAll(uint16_t m0,uint16_t m1,uint16_t m2,uint16_t m3) {
    if(!_armed||!_htim) return;
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_1,t2ccr(m0));
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_2,t2ccr(m1));
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_3,t2ccr(m2));
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_4,t2ccr(m3));
}

void Motor_Arm(void) {
    _armed=true;
    Motor_SetAll(MOTOR_ARM_SPIN,MOTOR_ARM_SPIN,MOTOR_ARM_SPIN,MOTOR_ARM_SPIN);
    HAL_Delay(200);
    Motor_SetAll(0,0,0,0);
}

void Motor_Disarm(void) {
    _armed=false;
    if(!_htim) return;
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_3,0);
    __HAL_TIM_SET_COMPARE(_htim,TIM_CHANNEL_4,0);
}

bool Motor_IsArmed(void) { return _armed; }

void Motor_Mix(uint16_t throttle, int16_t roll, int16_t pitch, int16_t yaw) {
    if(!_armed) return;
    int32_t t=throttle, r=roll/2, p=pitch/2, y=yaw/2;

    int32_t fl=t-r+p+y, fr=t+r+p-y, rl=t-r-p-y, rr=t+r-p+y;
    #define CL(v) ((v)<0?0:(v)>MOTOR_MAX?MOTOR_MAX:(v))
    Motor_SetAll((uint16_t)CL(fl),(uint16_t)CL(fr),
                 (uint16_t)CL(rl),(uint16_t)CL(rr));
    #undef CL
}
