#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "icm42688.h"
#include "bk2423.h"
#include "motor.h"
#include "sensor_fusion.h"
#include "flight_control.h"
/* USER CODE END Includes */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

#define IMU_INT_Pin          GPIO_PIN_0
#define IMU_INT_GPIO_Port    GPIOA
#define RF_CSN_Pin           GPIO_PIN_4
#define RF_CSN_GPIO_Port     GPIOA
#define RF_CE_Pin            GPIO_PIN_0
#define RF_CE_GPIO_Port      GPIOB
#define RF_IRQ_Pin           GPIO_PIN_1
#define RF_IRQ_GPIO_Port     GPIOB
#define IMU_CS_Pin           GPIO_PIN_12
#define IMU_CS_GPIO_Port     GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
