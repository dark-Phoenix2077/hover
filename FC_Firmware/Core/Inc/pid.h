#ifndef PID_H
#define PID_H
typedef struct {
    float kp,ki,kd, integral, prev_meas, ilim, olim;
} PID;
void  PID_Init(PID *p, float kp, float ki, float kd, float ilim, float olim);
float PID_Compute(PID *p, float setpoint, float measured, float dt);
void  PID_Reset(PID *p);
#endif
