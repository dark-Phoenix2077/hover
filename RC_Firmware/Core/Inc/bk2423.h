#ifndef BK2423_H
#define BK2423_H
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define NRF_CONFIG      0x00
#define NRF_EN_AA       0x01
#define NRF_EN_RXADDR   0x02
#define NRF_SETUP_AW    0x03
#define NRF_SETUP_RETR  0x04
#define NRF_RF_CH       0x05
#define NRF_RF_SETUP    0x06
#define NRF_STATUS      0x07
#define NRF_RX_ADDR_P0  0x0A
#define NRF_TX_ADDR     0x10
#define NRF_RX_PW_P0    0x11
#define NRF_FIFO_STATUS 0x17

#define CMD_R_REG    0x00
#define CMD_W_REG    0x20
#define CMD_R_RX_PLD 0x61
#define CMD_W_TX_PLD 0xA0
#define CMD_FLUSH_TX 0xE1
#define CMD_FLUSH_RX 0xE2

#define NRF_PAYLOAD_LEN 10

typedef struct __attribute__((packed)) {
    uint16_t throttle;
    int16_t  roll;
    int16_t  pitch;
    int16_t  yaw;
    uint8_t  flags;
    uint8_t  checksum;
} RC_Packet;

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *ce_port, *csn_port;
    uint16_t      ce_pin,   csn_pin;
} NRF_Handle;

void NRF_Init_RX(NRF_Handle *dev);
void NRF_Init_TX(NRF_Handle *dev);
bool NRF_DataReady(NRF_Handle *dev);
bool NRF_Receive(NRF_Handle *dev, RC_Packet *pkt);
bool NRF_Transmit(NRF_Handle *dev, RC_Packet *pkt);
#endif
