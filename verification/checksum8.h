#ifndef __CHECKSUM8_H__
#define __CHECKSUM8_H__

#ifdef  __cplusplus
extern "C" {
#endif

//exclusive or
extern unsigned char checksum8_xor(void *data, int offset, int length);

//summation
extern unsigned char checksum8_sum(void *data, int offset, int length);

#ifdef  __cplusplus
}
#endif

#endif
