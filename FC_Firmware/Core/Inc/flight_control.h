#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H
#include "bk2423.h"
#include "icm42688.h"
#include "sensor_fusion.h"
#include "motor.h"
#include "pid.h"
#include <stdbool.h>

typedef enum { FC_DISARMED, FC_ARMED, FC_FLYING,
               FC_AUTO_LAND, FC_FLIP } FC_State;

void     FC_Init(void);
void     FC_Update(const RC_Packet *pkt, const ICM_Data *imu,
                   const ICM_Offsets *off);
FC_State FC_GetState(void);
#endif
