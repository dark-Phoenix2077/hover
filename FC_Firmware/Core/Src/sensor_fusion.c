#include "sensor_fusion.h"
#include <math.h>
#include <stdbool.h>

#define RAD2DEG 57.2957795f
#define ALPHA   0.98f

void SF_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t SF_Micros(void) { return DWT->CYCCNT / 72; }

void SF_Update(const ICM_Data *raw, const ICM_Offsets *off, Attitude *att) {
    static uint32_t last_us = 0;
    static bool first = true;
    uint32_t now = SF_Micros();
    if (first) { last_us=now; first=false; att->roll=att->pitch=att->yaw=0; return; }
    float dt = (float)(now - last_us) * 1e-6f;
    last_us = now;

    float gx=raw->gx-off->gx, gy=raw->gy-off->gy, gz=raw->gz-off->gz;
    float ax=raw->ax-off->ax, ay=raw->ay-off->ay, az=raw->az-off->az;

    float ar = atan2f(ay,az)*RAD2DEG;
    float ap = -atan2f(ax,sqrtf(ay*ay+az*az))*RAD2DEG;

    att->roll  = ALPHA*(att->roll  + gx*dt) + (1.0f-ALPHA)*ar;
    att->pitch = ALPHA*(att->pitch + gy*dt) + (1.0f-ALPHA)*ap;
    att->yaw  += gz*dt;
    if(att->yaw> 180.0f) att->yaw-=360.0f;
    if(att->yaw<-180.0f) att->yaw+=360.0f;
}
