#include "flight_control.h"
#include <math.h>
#include <string.h>

#define RP_KP 0.6f
#define RP_KI 0.01f
#define RP_KD 0.02f
#define YR_KP 1.2f
#define YR_KI 0.005f
#define ANG_KP 4.0f

static PID pid_rr, pid_pr, pid_yr;
static PID pid_ra, pid_pa;
static FC_State state = FC_DISARMED;
static Attitude att;
static uint32_t last_pkt_ms = 0;

static uint32_t land_start_ms = 0;

static float flip_acc = 0;
static uint32_t flip_start_ms = 0;

#define FAILSAFE_MS 500

#ifdef DATA_LOGGING
static uint32_t log_tick=0;
if(++log_tick%2==0){
    char buf[96];
    int n=snprintf(buf,sizeof(buf),"%.3f,%.3f,%.3f,%.3f,%.4f,%.1f,%.1f\r\n",
        att.roll, att.pitch,
        imu->gx-off->gx, imu->gy-off->gy,
        (float)pkt->throttle/1000.0f,
        roll_out, pitch_out);
    HAL_UART_Transmit(&huart2,(uint8_t*)buf,n,5);
}
#endif

void FC_Init(void){
    PID_Init(&pid_rr, RP_KP,  RP_KI,  RP_KD,  200,500);
    PID_Init(&pid_pr, RP_KP,  RP_KI,  RP_KD,  200,500);
    PID_Init(&pid_yr, YR_KP,  YR_KI,  0,      200,500);
    PID_Init(&pid_ra, ANG_KP, 0,0,    1000,200);
    PID_Init(&pid_pa, ANG_KP, 0,0,    1000,200);
    memset(&att,0,sizeof(att));
    state=FC_DISARMED;
}


static void fly(const RC_Packet *pkt, const ICM_Data *imu,
                const ICM_Offsets *off, uint16_t thr, float dt){
    float rsp = (float)pkt->roll  *30.0f/500.0f;
    float psp = (float)pkt->pitch *30.0f/500.0f;
    float ysp = (float)pkt->yaw   *200.0f/500.0f;
    float rr_sp = PID_Compute(&pid_ra, rsp, att.roll,  dt);
    float pr_sp = PID_Compute(&pid_pa, psp, att.pitch, dt);
    float gx=imu->gx-off->gx, gy=imu->gy-off->gy, gz=imu->gz-off->gz;
    float ro = PID_Compute(&pid_rr, rr_sp, gx, dt);
    float po = PID_Compute(&pid_pr, pr_sp, gy, dt);
    float yo = PID_Compute(&pid_yr, ysp,   gz, dt);
    Motor_Mix(thr,(int16_t)ro,(int16_t)po,(int16_t)yo);
}



void FC_Update(const RC_Packet *pkt, const ICM_Data *imu,
               const ICM_Offsets *off){
    static uint32_t last_us = 0;
    uint32_t now_us = SF_Micros();
    float dt = (last_us==0) ? 0.001f : (float)(now_us-last_us)*1e-6f;
    last_us = now_us;

    SF_Update(imu, off, &att);

    uint32_t now_ms = HAL_GetTick();
    if(pkt) last_pkt_ms = now_ms;


    if((now_ms-last_pkt_ms)>FAILSAFE_MS && state!=FC_DISARMED){
        Motor_Disarm(); state=FC_DISARMED; return;
    }
    if(!pkt) return;

    switch(state){

    case FC_DISARMED:
        if((pkt->flags&0x01) && pkt->throttle<50){
            Motor_Arm();
            PID_Reset(&pid_rr); PID_Reset(&pid_pr); PID_Reset(&pid_yr);
            PID_Reset(&pid_ra); PID_Reset(&pid_pa);
            state=FC_ARMED;
        }
        break;

    case FC_ARMED:
        if(!(pkt->flags&0x01)){ Motor_Disarm(); state=FC_DISARMED; break; }
        if(pkt->throttle>100) state=FC_FLYING;
        Motor_SetAll(0,0,0,0);
        break;

    case FC_FLYING:
        if(!(pkt->flags&0x01)){ Motor_Disarm(); state=FC_DISARMED; break; }
        if(pkt->flags&0x04){ land_start_ms=now_ms; state=FC_AUTO_LAND; break; }
        if((pkt->flags&0x02) && pkt->throttle>300){
            flip_acc=0; flip_start_ms=now_ms; state=FC_FLIP; break;
        }
        fly(pkt,imu,off,pkt->throttle,dt);
        break;

    case FC_AUTO_LAND:{
        uint32_t el  = now_ms - land_start_ms;
        float prog   = (float)el/2500.0f;
        if(prog>1.0f) prog=1.0f;
        uint16_t thr = (uint16_t)((1.0f-prog)*400.0f);
        float az     = imu->az - off->az;
        if((fabsf(az)<0.4f && prog>0.5f) || prog>=1.0f){
            Motor_Disarm(); state=FC_DISARMED; break;
        }

        float gx=imu->gx-off->gx, gy=imu->gy-off->gy;
        float rr_sp=PID_Compute(&pid_ra,0,att.roll,dt);
        float pr_sp=PID_Compute(&pid_pa,0,att.pitch,dt);
        float ro=PID_Compute(&pid_rr,rr_sp,gx,dt);
        float po=PID_Compute(&pid_pr,pr_sp,gy,dt);
        Motor_Mix(thr,(int16_t)ro,(int16_t)po,0);
        break;
    }

    case FC_FLIP:{
        uint32_t el = now_ms - flip_start_ms;
        float gx    = imu->gx - off->gx;
        flip_acc   += fabsf(gx)*dt;
        if(flip_acc>=360.0f || el>800){
            PID_Reset(&pid_rr); state=FC_FLYING; break;
        }
        Motor_Mix(800,500,0,0);
        break;
    }
    }
}

FC_State FC_GetState(void){ return state; }
