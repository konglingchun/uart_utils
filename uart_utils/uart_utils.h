#ifndef _UART_INIT_H_
#define _UART_INIT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <termios.h>

extern int uart_open(char *dev_name);

extern void uart_print_attr(struct termios *options);

/*************************************************************
* 功能：	设置串口属性结构体
* 参数：	fd：串口设备文件描述符
			speed：串口波特率（可选2400、4800、9600、115200。可自己酌情修改程序添加支持）
			data_bits：数据位宽（可选5、6、7、8）
			stop_bits：停止位（可选1、2）
			check：奇偶校验位（可选'N'、'O'、'E'；分别应着无奇偶校验、奇校验、偶校验）
			flow_ctrl：硬件流控制（0为OFF，1为ON）
			options：串口属结构体，
* 返回值：	串口属性结构体
**************************************************************/
extern struct termios *uart_set_attr(int fd,
				int speed,
				int data_bits,
				int stop_bits,
				int check, 
				int flow_ctrl,
				struct termios *options);

/*************************************************************
* 功能：	设置回显
* 参数：	串口设备文件名
		echo：回显（0为关，1为开）
* 返回值：	无
**************************************************************/
extern void uart_set_echo(int uart_fd, int echo);

/*************************************************************
* 功能：	设置阻塞
* 参数：	串口设备文件名
		block：阻塞（0为不阻塞，1为阻塞）
* 返回值：	无
**************************************************************/
extern void uart_set_block(int uart_fd, int block);

/*************************************************************
* 功能：	串口初始化程序
* 参数：	串口设备文件名
		speed：串口波特率（可选2400、4800、9600、115200。可自己酌情修改程序添加支持）
		data_bits：数据位宽（可选5、6、7、8）
		stop_bits：停止位（可选1、2）
		check：奇偶校验位（可选'N'、'O'、'E'；分别应着无奇偶校验、奇校验、偶校验）
		flow_ctrl：硬件流控制（0为OFF，1为ON）
* 返回值：	串口设备文件描述符
**************************************************************/
extern int uart_init(int uart_fd,
			  	int speed,
				int data_bits,
				int stop_bits,
				int check, 
				int flow_ctrl);

/*************************************************************
* 功能：	串口反初始化程序
* 参数：	串口设备文件描述符
* 返回值：	无
**************************************************************/
extern void uart_uninit(int uart_fd);

/*************************************************************
* 功能：	串口发送字符串
* 参数：	uart_fd：串口设备文件描述符
			str：待发送的字符
* 返回值：	无
**************************************************************/
extern void uart_send_str(int uart_fd, char *str);

/*************************************************************
* 功能：	读一串至截止字符的字符串，在设定的时间内读不到数据则函数返回
* 参数：	uart_fd：串口设备文件描述符
			buffer：存放数据的内存的首地址
			len：存放数据的内存空间的大小
			until：截止字符
			timeout_ms：超时时间(单位：ms)
* 返回值：	
			成功：实际读到的字符数
			失败：-1
**************************************************************/
extern int uart_read_until_char(int uart_fd, char *buffer, int len, unsigned char until, int timeout_ms);

/*************************************************************
* 功能：	设定的时间内读取数据
* 参数：	uart_fd：串口设备文件描述符
			buffer：存放数据的内存的首地址
			len：存放数据的内存空间的大小
			timeout_ms：超时时间(单位：ms)
* 返回值：	
			成功：实际读到的字符数
			失败：-1
**************************************************************/
extern int uart_read_until_time(int uart_fd, char *buffer, int len, int timeout_first, int timeout_interval);

#ifdef  __cplusplus
}
#endif

#endif
