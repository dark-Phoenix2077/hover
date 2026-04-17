#include "main.h"
#include "bk2423.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;
extern ADC_HandleTypeDef hadc;

static NRF_Handle nrf = {
    &hspi1,
    RF_CE_GPIO_Port,  RF_CE_Pin,
    RF_CSN_GPIO_Port, RF_CSN_Pin
};

static uint16_t adc_buf[5];

typedef struct { GPIO_TypeDef *port; uint16_t pin; } BtnDef;
static const BtnDef BTNS[8] = {
    {BTN_ARM_GPIO_Port,       BTN_ARM_Pin      },
    {BTN_FLIP_GPIO_Port,      BTN_FLIP_Pin     },
    {BTN_LAND_GPIO_Port,      BTN_LAND_Pin     },
    {BTN_TKOFF_GPIO_Port,     BTN_TKOFF_Pin    },
    {BTN_RTH_GPIO_Port,       BTN_RTH_Pin      },
    {BTN_SP1_GPIO_Port,       BTN_SP1_Pin      },
    {BTN_SP2_GPIO_Port,       BTN_SP2_Pin      },
    {BTN_SP3_GPIO_Port,       BTN_SP3_Pin      },
};

static uint8_t  btn_state[8], btn_prev[8];
static uint32_t btn_timer[8];
#define DEBOUNCE_MS 20
#define BATT_LOW_THRESH 620

static void debounce_buttons(void){
    uint32_t now=HAL_GetTick();
    for(int i=0;i<8;i++){
        uint8_t raw=(HAL_GPIO_ReadPin(BTNS[i].port,BTNS[i].pin)==GPIO_PIN_RESET)?1:0;
        if(raw!=btn_prev[i]){ btn_timer[i]=now; btn_prev[i]=raw; }
        if(now-btn_timer[i]>=DEBOUNCE_MS) btn_state[i]=raw;
    }
}

static int16_t stick(uint16_t raw, bool inv){
    int32_t v=((int32_t)raw-2047)*500/2047;
    v=(v>500)?500:(v<-500)?-500:v;
    return inv?(int16_t)(-v):(int16_t)v;
}
static uint16_t throttle(uint16_t raw){
    uint32_t t=(uint32_t)raw*1000/4095;
    return (uint16_t)(t>1000?1000:t);
}

void rc_main(void){
    NRF_Init_TX(&nrf);
    HAL_ADC_Start_DMA(&hadc,(uint32_t*)adc_buf,5);
    memset(btn_state,0,sizeof(btn_state));
    memset(btn_prev, 0,sizeof(btn_prev));
    memset(btn_timer,0,sizeof(btn_timer));

    bool armed=false, prev_arm=false;
    uint32_t last_tx=0, last_beep=0;
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);

    while(1){
        debounce_buttons();

        if(btn_state[0] && !prev_arm) armed=!armed;
        prev_arm=btn_state[0];

        if(adc_buf[4]<BATT_LOW_THRESH){
            uint32_t now=HAL_GetTick();
            if(now-last_beep>2000){
                HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
                HAL_Delay(80);
                HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
                last_beep=now;
            }
        }

        /* 100 Hz TX loop */
        if(HAL_GetTick()-last_tx>=10){
            last_tx=HAL_GetTick();
            RC_Packet pkt;
            pkt.throttle = throttle(adc_buf[0]);
            pkt.yaw      = stick(adc_buf[1],false);
            pkt.pitch    = stick(adc_buf[2],true);
            pkt.roll     = stick(adc_buf[3],false);
            pkt.flags    = 0;
            if(armed)         pkt.flags|=0x01;
            if(btn_state[1])  pkt.flags|=0x02;
            if(btn_state[2])  pkt.flags|=0x04;
            NRF_Transmit(&nrf,&pkt);
            HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
        }
    }
}
