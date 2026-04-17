#ifndef ICM42688_H
#define ICM42688_H
#include "stm32f1xx_hal.h"
#include <stdint.h>

#define ICM_DEVICE_CONFIG  0x11
#define ICM_INT_CONFIG     0x14
#define ICM_ACCEL_DATA_X1  0x1F
#define ICM_PWR_MGMT0      0x4E
#define ICM_GYRO_CONFIG0   0x4F
#define ICM_ACCEL_CONFIG0  0x50
#define ICM_GYRO_CONFIG1   0x51
#define ICM_ACCEL_CONFIG1  0x53
#define ICM_INT_SOURCE0    0x65
#define ICM_WHO_AM_I       0x75
#define ICM_WHOAMI_VAL     0x47

#define ICM_GYRO_SENS      65.5f
#define ICM_ACCEL_SENS     8192.0f

typedef struct { float ax,ay,az, gx,gy,gz, temp; } ICM_Data;
typedef struct { float ax,ay,az, gx,gy,gz;        } ICM_Offsets;
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t           cs_pin;
} ICM_Handle;

HAL_StatusTypeDef ICM_Init(ICM_Handle *dev);
HAL_StatusTypeDef ICM_Read(ICM_Handle *dev, ICM_Data *out);
void              ICM_Calibrate(ICM_Handle *dev, ICM_Offsets *off);
#endif
