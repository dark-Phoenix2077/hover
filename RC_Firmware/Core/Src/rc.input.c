#ifndef RC_INPUT_H
#define RC_INPUT_H

#include <stdint.h>
#include "rc_packet.h"

void RC_Input_Update(RC_Packet *pkt, const uint16_t adc_raw[5]);

#endif
