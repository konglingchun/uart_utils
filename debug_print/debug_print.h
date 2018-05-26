#ifndef __DEBUG_PRINT__
#define __DEBUG_PRINT__

#ifdef  __cplusplus
extern "C" {
#endif

#define DEBUG		0
#define INFO		1
#define WARN		2
#define ERROR		3
#define FATAL		4
#define NOLOG		5
#define LEVEL_MAX	6

extern int debug_print(int level, char *name, long line, const char *function, char *format , ...);

#define printd(level, fmt, args...)\
   do{ \
		debug_print(level, __FILE__, __LINE__, __FUNCTION__, fmt, ##args);\
   }while(0)

#ifdef  __cplusplus
}
#endif

#endif
