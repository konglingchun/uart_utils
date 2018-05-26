#ifndef __CRC16_H__
#define __CRC16_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint16_t crc16_calculate(uint8_t *buffer, uint16_t buffer_length);

extern int crc16_check(uint8_t *buffer, int size, int crc_length);

extern int crc16_append(uint8_t *buffer, int size, int crc_length);

#ifdef  __cplusplus
}
#endif

#endif
