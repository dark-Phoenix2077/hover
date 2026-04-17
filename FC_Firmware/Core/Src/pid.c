#include "pid.h"

void PID_Init(PID *p,float kp,float ki,float kd,float ilim,float olim){
    p->kp=kp; p->ki=ki; p->kd=kd; p->ilim=ilim; p->olim=olim;
    PID_Reset(p);
}
float PID_Compute(PID *p, float sp, float meas, float dt){
    float err = sp - meas;
    p->integral += err*dt;
    if(p->integral> p->ilim) p->integral= p->ilim;
    if(p->integral<-p->ilim) p->integral=-p->ilim;
    float out = p->kp*err + p->ki*p->integral
              - p->kd*(meas - p->prev_meas)/dt;
    p->prev_meas = meas;
    if(out> p->olim) out= p->olim;
    if(out<-p->olim) out=-p->olim;
    return out;
}
void PID_Reset(PID *p){ p->integral=0; p->prev_meas=0; }
