#ifndef ML_INFERENCE_H
#define ML_INFERENCE_H
#include <stdint.h>

void     ML_Init(void);

void     ML_Infer(float roll,   float pitch,
                  float gx,     float gy,
                  float throttle,
                  float *roll_correction,
                  float *pitch_correction);

#define ML_BLEND 0.3f

#endif
