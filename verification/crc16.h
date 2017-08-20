#ifndef __CRC16_H__
#define __CRC16_H__

#include <stdint.h>

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);

int crc_send_msg_pre(uint8_t *req, int req_length);

#endif
