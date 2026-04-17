#ifndef SENSOR_FUSION_H
#define SENSOR_FUSION_H
#include "icm42688.h"

typedef struct { float roll, pitch, yaw; } Attitude;

void     SF_Init(void);
void     SF_Update(const ICM_Data *raw, const ICM_Offsets *off, Attitude *att);
uint32_t SF_Micros(void);
#endif
