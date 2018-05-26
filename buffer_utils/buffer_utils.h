#ifndef __BUFFER_UTILS_H__
#define __BUFFER_UTILS_H__

#ifdef  __cplusplus
extern "C" {
#endif

extern void print_buffer(char *pre, char *buffer, int size);

extern void print_buffer_char(char *pre, char *buffer, int size);

extern void print_buffer_char_index(char *pre, char *buffer, int size);

extern void print_buffer_hex(char *pre, char *buffer, int size);

extern void print_buffer_hex_index(char *pre, char *buffer, int size);

#ifdef  __cplusplus
}
#endif

#endif
