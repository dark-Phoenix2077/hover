
#include "bk2423.h"
#include <string.h>

static const uint8_t PIPE_ADDR[5] = {0xA3,0xB2,0xC1,0xD0,0xE7};

static void ce_hi (NRF_Handle *d){HAL_GPIO_WritePin(d->ce_port, d->ce_pin, GPIO_PIN_SET);  }
static void ce_lo (NRF_Handle *d){HAL_GPIO_WritePin(d->ce_port, d->ce_pin, GPIO_PIN_RESET);}
static void csn_hi(NRF_Handle *d){HAL_GPIO_WritePin(d->csn_port,d->csn_pin,GPIO_PIN_SET);  }
static void csn_lo(NRF_Handle *d){HAL_GPIO_WritePin(d->csn_port,d->csn_pin,GPIO_PIN_RESET);}

static uint8_t nrf_rr(NRF_Handle *d, uint8_t reg) {
    uint8_t cmd=CMD_R_REG|(reg&0x1F), val=0;
    csn_lo(d); HAL_SPI_Transmit(d->hspi,&cmd,1,10); HAL_SPI_Receive(d->hspi,&val,1,10); csn_hi(d);
    return val;
}
static void nrf_wr(NRF_Handle *d, uint8_t reg, uint8_t val) {
    uint8_t b[2]={CMD_W_REG|(reg&0x1F),val};
    csn_lo(d); HAL_SPI_Transmit(d->hspi,b,2,10); csn_hi(d);
}
static void nrf_wr_buf(NRF_Handle *d, uint8_t reg, const uint8_t *src, uint8_t len) {
    uint8_t cmd=CMD_W_REG|(reg&0x1F);
    csn_lo(d); HAL_SPI_Transmit(d->hspi,&cmd,1,10); HAL_SPI_Transmit(d->hspi,(uint8_t*)src,len,10); csn_hi(d);
}
static void nrf_cmd(NRF_Handle *d, uint8_t cmd) {
    csn_lo(d); HAL_SPI_Transmit(d->hspi,&cmd,1,10); csn_hi(d);
}
static void nrf_clear(NRF_Handle *d) { nrf_wr(d,NRF_STATUS,0x70); }

static void nrf_base(NRF_Handle *d) {
    ce_lo(d); csn_hi(d); HAL_Delay(5);
    nrf_wr(d,NRF_EN_AA,     0x01);
    nrf_wr(d,NRF_EN_RXADDR, 0x01);
    nrf_wr(d,NRF_SETUP_AW,  0x03);
    nrf_wr(d,NRF_SETUP_RETR,0x2F);
    nrf_wr(d,NRF_RF_CH,     0x4E);
    nrf_wr(d,NRF_RF_SETUP,  0x0E);
    nrf_wr_buf(d,NRF_RX_ADDR_P0,PIPE_ADDR,5);
    nrf_wr_buf(d,NRF_TX_ADDR,   PIPE_ADDR,5);
    nrf_wr(d,NRF_RX_PW_P0,NRF_PAYLOAD_LEN);
    nrf_clear(d);
    nrf_cmd(d,CMD_FLUSH_TX); nrf_cmd(d,CMD_FLUSH_RX);
}

void NRF_Init_RX(NRF_Handle *dev) {
    nrf_base(dev);
    nrf_wr(dev,NRF_CONFIG,0x0F);
    HAL_Delay(2); ce_hi(dev);
}
void NRF_Init_TX(NRF_Handle *dev) {
    nrf_base(dev);
    nrf_wr(dev,NRF_CONFIG,0x0E);
    HAL_Delay(2);
}

bool NRF_DataReady(NRF_Handle *dev) {
    return !(nrf_rr(dev,NRF_FIFO_STATUS) & 0x01);
}

bool NRF_Receive(NRF_Handle *dev, RC_Packet *pkt) {
    if (!NRF_DataReady(dev)) return false;
    uint8_t cmd=CMD_R_RX_PLD, buf[NRF_PAYLOAD_LEN];
    csn_lo(dev); HAL_SPI_Transmit(dev->hspi,&cmd,1,10);
    HAL_SPI_Receive(dev->hspi,buf,NRF_PAYLOAD_LEN,20); csn_hi(dev);
    nrf_clear(dev);
    uint8_t csum=0;
    for(int i=0;i<NRF_PAYLOAD_LEN-1;i++) csum^=buf[i];
    if(csum!=buf[NRF_PAYLOAD_LEN-1]) return false;
    memcpy(pkt,buf,sizeof(RC_Packet));
    return true;
}

bool NRF_Transmit(NRF_Handle *dev, RC_Packet *pkt) {
    uint8_t *b=(uint8_t*)pkt; pkt->checksum=0;
    for(int i=0;i<NRF_PAYLOAD_LEN-1;i++) pkt->checksum^=b[i];
    nrf_cmd(dev,CMD_FLUSH_TX); nrf_clear(dev);
    uint8_t cmd=CMD_W_TX_PLD;
    csn_lo(dev); HAL_SPI_Transmit(dev->hspi,&cmd,1,10);
    HAL_SPI_Transmit(dev->hspi,(uint8_t*)pkt,NRF_PAYLOAD_LEN,20); csn_hi(dev);
    ce_hi(dev); HAL_Delay(1); ce_lo(dev);
    uint32_t t=HAL_GetTick();
    while(HAL_GetTick()-t<10){
        uint8_t s=nrf_rr(dev,NRF_STATUS);
        if(s&0x20) return true;
        if(s&0x10) return false;
    }
    return false;
}
