#include "rc_input.h"
#include "main.h"


#define DEADBAND     40


static inline uint16_t clamp16(int32_t v, int32_t lo, int32_t hi) {
    if (v < lo) return (uint16_t)lo;
    if (v > hi) return (uint16_t)hi;
    return (uint16_t)v;
}

static uint16_t apply_deadband(uint16_t raw) {
    int32_t v = (int32_t)raw - 2047;
    if (v >  DEADBAND) v -= DEADBAND;
    else if (v < -DEADBAND) v += DEADBAND;
    else v = 0;
    return clamp16(v + 2047, 0, 4095);
}

static uint8_t read_buttons(void) {
    uint8_t b = 0;
    if (HAL_GPIO_ReadPin(BTN_ARM_GPIO_Port,   BTN_ARM_Pin)   == GPIO_PIN_RESET) b |= BTN_ARM;
    if (HAL_GPIO_ReadPin(BTN_FLIP_GPIO_Port,  BTN_FLIP_Pin)  == GPIO_PIN_RESET) b |= BTN_FLIP;
    if (HAL_GPIO_ReadPin(BTN_LAND_GPIO_Port,  BTN_LAND_Pin)  == GPIO_PIN_RESET) b |= BTN_LAND;
    if (HAL_GPIO_ReadPin(BTN_TKOFF_GPIO_Port, BTN_TKOFF_Pin) == GPIO_PIN_RESET) b |= BTN_TKOFF;
    if (HAL_GPIO_ReadPin(BTN_RTH_GPIO_Port,   BTN_RTH_Pin)   == GPIO_PIN_RESET) b |= BTN_RTH;
    if (HAL_GPIO_ReadPin(BTN_SP1_GPIO_Port,   BTN_SP1_Pin)   == GPIO_PIN_RESET) b |= BTN_SP1;
    if (HAL_GPIO_ReadPin(BTN_SP2_GPIO_Port,   BTN_SP2_Pin)   == GPIO_PIN_RESET) b |= BTN_SP2;
    if (HAL_GPIO_ReadPin(BTN_SP3_GPIO_Port,   BTN_SP3_Pin)   == GPIO_PIN_RESET) b |= BTN_SP3;
    return b;
}

void RC_Input_Update(RC_Packet *pkt, const uint16_t adc_raw[5]) {
    pkt->throttle = clamp16(adc_raw[0], 0, 4095);

    pkt->yaw   = apply_deadband(adc_raw[1]);
    pkt->pitch = apply_deadband(adc_raw[2]);
    pkt->roll  = apply_deadband(adc_raw[3]);

    pkt->vbat = (uint8_t)(adc_raw[4] >> 4);

    pkt->buttons = read_buttons();
}
