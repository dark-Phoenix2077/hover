#include "icm42688.h"

static void cs_lo(ICM_Handle *d) { HAL_GPIO_WritePin(d->cs_port,d->cs_pin,GPIO_PIN_RESET); }
static void cs_hi(ICM_Handle *d) { HAL_GPIO_WritePin(d->cs_port,d->cs_pin,GPIO_PIN_SET);   }

static uint8_t icm_rr(ICM_Handle *d, uint8_t reg) {
    uint8_t cmd=reg|0x80, val=0;
    cs_lo(d); HAL_SPI_Transmit(d->hspi,&cmd,1,10); HAL_SPI_Receive(d->hspi,&val,1,10); cs_hi(d);
    return val;
}
static void icm_wr(ICM_Handle *d, uint8_t reg, uint8_t val) {
    uint8_t b[2]={reg&0x7F,val};
    cs_lo(d); HAL_SPI_Transmit(d->hspi,b,2,10); cs_hi(d);
}
static void icm_burst(ICM_Handle *d, uint8_t reg, uint8_t *dst, uint8_t len) {
    uint8_t cmd=reg|0x80;
    cs_lo(d); HAL_SPI_Transmit(d->hspi,&cmd,1,10); HAL_SPI_Receive(d->hspi,dst,len,20); cs_hi(d);
}

HAL_StatusTypeDef ICM_Init(ICM_Handle *dev) {
    cs_hi(dev); HAL_Delay(10);
    icm_wr(dev, ICM_DEVICE_CONFIG, 0x01); HAL_Delay(2);
    if (icm_rr(dev, ICM_WHO_AM_I) != ICM_WHOAMI_VAL) return HAL_ERROR;
    icm_wr(dev, ICM_PWR_MGMT0,     0x0F); HAL_Delay(1);
    icm_wr(dev, ICM_GYRO_CONFIG0,  0x46);
    icm_wr(dev, ICM_ACCEL_CONFIG0, 0x46);
    icm_wr(dev, ICM_GYRO_CONFIG1,  0x16);
    icm_wr(dev, ICM_ACCEL_CONFIG1, 0x06);
    icm_wr(dev, ICM_INT_CONFIG,    0x18);
    icm_wr(dev, ICM_INT_SOURCE0,   0x08);
    HAL_Delay(50);
    return HAL_OK;
}

HAL_StatusTypeDef ICM_Read(ICM_Handle *dev, ICM_Data *out) {
    uint8_t r[14];
    icm_burst(dev, ICM_ACCEL_DATA_X1, r, 14);
    int16_t rax=(int16_t)((r[0]<<8)|r[1]),  ray=(int16_t)((r[2]<<8)|r[3]);
    int16_t raz=(int16_t)((r[4]<<8)|r[5]),  rgx=(int16_t)((r[6]<<8)|r[7]);
    int16_t rgy=(int16_t)((r[8]<<8)|r[9]),  rgz=(int16_t)((r[10]<<8)|r[11]);
    int16_t rt =(int16_t)((r[12]<<8)|r[13]);
    out->ax=(float)rax/ICM_ACCEL_SENS; out->ay=(float)ray/ICM_ACCEL_SENS;
    out->az=(float)raz/ICM_ACCEL_SENS; out->gx=(float)rgx/ICM_GYRO_SENS;
    out->gy=(float)rgy/ICM_GYRO_SENS;  out->gz=(float)rgz/ICM_GYRO_SENS;
    out->temp=((float)rt/132.48f)+25.0f;
    return HAL_OK;
}

void ICM_Calibrate(ICM_Handle *dev, ICM_Offsets *off) {
    ICM_Data s; double sgx=0,sgy=0,sgz=0,sax=0,say=0,saz=0;
    for (int i=0;i<2000;i++) {
        ICM_Read(dev,&s);
        sgx+=s.gx; sgy+=s.gy; sgz+=s.gz;
        sax+=s.ax; say+=s.ay; saz+=s.az;
        HAL_Delay(1);
    }
    off->gx=(float)(sgx/2000); off->gy=(float)(sgy/2000); off->gz=(float)(sgz/2000);
    off->ax=(float)(sax/2000); off->ay=(float)(say/2000); off->az=(float)(saz/2000)-1.0f;
}
