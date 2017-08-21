#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

#include "debug_print.h"

static char	log_level_str[][LEVEL_MAX] = { "DEBUG" , "INFO" , "WARN" , "ERROR" , "FATAL" , "NOLOG" } ;

static void debug_get_time(char *buffer, int len)
{
	struct timeval tv;
	struct tm result;

	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &result);
	snprintf(buffer, len, "%04d-%02d-%02d_%d-%d-%d",
	result.tm_year + 1900, result.tm_mon + 1, result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);
}

int debug_print(int level, char *name, long line, const char *function, char *format , ...)
{
	va_list	valist;
	char buffer[512] = "";
	char time_buffer[128] = "";
	char buffer_result[512] = "";
	char buffer_result_length = 0;
	int ret;
	
	va_start(valist, format);
	ret = vsnprintf(buffer, sizeof(buffer), format, valist);
	va_end(valist);
	debug_get_time(time_buffer, sizeof(time_buffer));
	if(level < 0){
		level = 0;
	}
	level = level%LEVEL_MAX;
	switch (level){
	case DEBUG:
		buffer_result_length = snprintf(buffer_result, sizeof(buffer_result), "%s|%s|%s +%ld|%s|%s"
	 		,log_level_str[level]
	 		,time_buffer
			,name
			,line
	 		,function
	 		,buffer);
		break;
	case INFO:
		buffer_result_length = snprintf(buffer_result, sizeof(buffer_result), "%s|%s|%s"
			,log_level_str[level]
			,time_buffer
			,buffer);
		break;
	case WARN:
		buffer_result_length = snprintf(buffer_result, sizeof(buffer_result), "%s|%s|%s|%s"
	 		,log_level_str[level]
	 		,time_buffer
	 		,function
	 		,buffer);
		break;
	case ERROR:
	case FATAL:
		buffer_result_length = snprintf(buffer_result, sizeof(buffer_result), "%s|%s|%s +%ld|%s|%s"
	 		,log_level_str[level]
	 		,time_buffer
			,name
			,line
	 		,function
	 		,buffer);
		break;
	}
	printf("%s", buffer_result);
	return ret;
}

