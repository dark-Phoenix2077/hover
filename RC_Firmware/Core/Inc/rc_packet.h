#ifndef RC_PACKET_H
#define RC_PACKET_H

#include <stdint.h>

typedef struct {
    uint16_t throttle;
    uint16_t yaw;
    uint16_t pitch;
    uint16_t roll;
    uint8_t  buttons;
    uint8_t  vbat;
} RC_Packet;


#define BTN_ARM    (1 << 0)
#define BTN_FLIP   (1 << 1)
#define BTN_LAND   (1 << 2)
#define BTN_TKOFF  (1 << 3)
#define BTN_RTH    (1 << 4)
#define BTN_SP1    (1 << 5)
#define BTN_SP2    (1 << 6)
#define BTN_SP3    (1 << 7)

#endif
