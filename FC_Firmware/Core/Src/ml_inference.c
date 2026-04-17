#include "ml_inference.h"

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "stabilizer_model.c"

#include "norm_stats.h"

constexpr int kArenaSize = 4096;
static uint8_t tensor_arena[kArenaSize];
static tflite::MicroInterpreter *interp = nullptr;

void ML_Init(void){
    static tflite::MicroErrorReporter err;
    static tflite::AllOpsResolver    resolver;
    static const tflite::Model *model =
        tflite::GetModel(stabilizer_model_tflite);
    static tflite::MicroInterpreter static_interp(
        model, resolver, tensor_arena, kArenaSize,
        &err);
    interp = &static_interp;
    interp->AllocateTensors();
}

static inline int8_t quantize(float v, float mean, float std,
                               float scale, int32_t zp){
    float norm = (v - mean) / std;
    int32_t q  = (int32_t)((norm / scale) + zp);
    return (int8_t)(q < -128 ? -128 : q > 127 ? 127 : q);
}
static inline float dequantize(int8_t q, float scale, int32_t zp){
    return ((float)(q - zp)) * scale;
}

void ML_Infer(float roll, float pitch, float gx, float gy, float thr,
              float *r_out, float *p_out){
    if(!interp){ *r_out=*p_out=0; return; }
    TfLiteTensor *in  = interp->input(0);
    TfLiteTensor *out = interp->output(0);
    float is = in->params.scale; int32_t iz = in->params.zero_point;
    in->data.int8[0] = quantize(roll,  XMEAN_0, XSTD_0, is, iz);
    in->data.int8[1] = quantize(pitch, XMEAN_1, XSTD_1, is, iz);
    in->data.int8[2] = quantize(gx,    XMEAN_2, XSTD_2, is, iz);
    in->data.int8[3] = quantize(gy,    XMEAN_3, XSTD_3, is, iz);
    in->data.int8[4] = quantize(thr,   XMEAN_4, XSTD_4, is, iz);
    interp->Invoke();
    float os = out->params.scale; int32_t oz = out->params.zero_point;
    *r_out = dequantize(out->data.int8[0], os, oz) * YSTD_0 + YMEAN_0;
    *p_out = dequantize(out->data.int8[1], os, oz) * YSTD_1 + YMEAN_1;
}
