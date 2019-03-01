#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/select.h>

#include "debug_print.h"

static struct termios option_old;

#define STRING(x) #x
#define TO_STRING(x) STRING(x)

#define S(x)    X(x,B##x,TO_STRING(x))
#define VALID_TERMINAL_SPEED_TABLE \
        S(0)\
        S(50)\
        S(75)\
        S(110)\
        S(134)\
        S(150)\
        S(200)\
        S(300)\
        S(600)\
        S(1200)\
        S(1800)\
        S(2400)\
        S(4800)\
        S(9600)\
        S(19200)\
        S(38400)\
        S(57600)\
        S(115200)\
        S(230400)\
        S(460800)\
        S(500000)\
        S(576000)\
        S(921600)\
        S(1000000)\
        S(1152000)\
        S(1500000)\
        S(2000000)\
        S(2500000)\
        S(3000000)\
        S(3500000)\
        S(4000000)

/*************************************************************
* 功能：	打开串口设备文件
* 参数：	串口设备文件名
* 返回值：	串口设备文件描述符
**************************************************************/
int uart_open(char *dev_name)
{
	int uart_fd;
	
	uart_fd = open(dev_name, O_RDWR|O_NOCTTY);
	if(uart_fd < 0)
	{
		printd(ERROR, "uart_open\n");
		_exit(-1);
	}
	return uart_fd;
}

void uart_print_attr(struct termios *options)
{
	int i;
	
	printd(INFO, "c_iflag = 0%o\n", options->c_iflag);
	printd(INFO, "c_oflag = 0%o\n", options->c_oflag);
	printd(INFO, "c_cflag = 0%o\n", options->c_cflag);
	printd(INFO, "c_lflag = 0%o\n", options->c_lflag);
	for(i=VINTR;i<=VEOL2;i++)
	{
		printf("c_cc[%d] = %d\n", i, options->c_cc[i]);
	}
}

/*************************************************************
* 功能：	串口默认属性
* 参数：	无
* 返回值：	无
**************************************************************/
static struct termios *uart_default_attr(void)
{
	struct termios *options = NULL;
	
	options = (struct termios *)calloc(1, sizeof(struct termios));
	if(options == NULL)
	{
		printd(ERROR, "calloc");
		_exit(-1);
	}
	/*
	cfmakeraw() sets the terminal to something like the "raw" mode of the old Version 7 terminal driver:
	input is available character by character, echoing is disabled, and all special processing of terminal input
	and output characters is disabled.	The terminal attributes are set as follows:
	*/
	cfmakeraw(options);
    /* Ignore break characters */
    options->c_iflag = IGNBRK;
    /* Enable receiver.  */
    options->c_cflag |= CREAD;
    /* Ignore modem status lines.  */
	options->c_cflag |= CLOCAL;
	/* Number of bits per byte (mask).  */
	options->c_cflag &= ~CSIZE;
    /* Minimum number of bytes read at once [!ICANON].  */
	options->c_cc[VMIN] = 1;
	/* Time-out value (tenths of a second) [!ICANON].  */
	options->c_cc[VTIME] = 0;
	return options;
}

static inline const char* valid_terminal_speeds(void)
{
#define X(x,y,z) "\n\t " z
    return VALID_TERMINAL_SPEED_TABLE;
#undef X
}

static inline speed_t check_speed_parameter(int requested_speed)
{
    switch(requested_speed)
    {
#define X(x,y,z) case x: return y;
        VALID_TERMINAL_SPEED_TABLE
    default:
        fprintf(stderr,"invalid speed parameters, valid values are %s\n", valid_terminal_speeds());
        return B9600;
#undef X
    }
}

/*************************************************************
* 功能：	设置串口属性结构体
* 参数：	
			speed：串口波特率（可选2400、4800、9600、115200。可自己酌情修改程序添加支持）
			data_bits：数据位宽（可选5、6、7、8）
			stop_bits：停止位（可选1、2）
			check：奇偶校验位（可选'N'、'O'、'E'；分别应着无奇偶校验、奇校验、偶校验）
			flow_ctrl：硬件流控制（0为OFF，1为ON）
			options：串口属结构体，
* 返回值：	串口属性结构体
**************************************************************/
struct termios *uart_set_attr(
				int speed,
				int data_bits,
				int stop_bits,
				int check, 
				int flow_ctrl,
				struct termios *options)
{
	speed_t baud_rate = 0;
	
	if(options == NULL)
	{
		options = uart_default_attr();
	}
	switch(data_bits)     
	{   
		case 5:     
			options->c_cflag |= CS5;     
			break; 
		case 6:     
			options->c_cflag |= CS6;     
			break; 			
		case 7:     
			options->c_cflag |= CS7;     
			break;     
		case 8:     
			options->c_cflag |= CS8;     
			break;
		default:
			options->c_cflag |= CS8;
			break;
	}
	/****************校验位选择****************/    
	switch(check)     
	{     
        case 'O':
			options->c_cflag |= PARENB;//允许输出产生奇偶信息以及输入的奇偶校验
			options->c_cflag |= PARODD;//奇校验
			options->c_iflag |= (INPCK|ISTRIP);//INPCK:启用输入奇偶检测;ISTRIP:去掉第八位(传输时只传7位)
			break;     
        case 'E':
            options->c_cflag |= PARENB;//允许输出产生奇偶信息以及输入的奇偶校验
			options->c_cflag &= ~PARODD;//输入和输出是偶校验(ECC)
			options->c_iflag |= (INPCK|ISTRIP);
			break;     
		case 'N':     
			options->c_cflag &= ~PARENB;//无奇偶校验位
	}
	/****************硬件流控制****************/
	switch (flow_ctrl)
	{
		case '0'://OFF
			options->c_cflag &= ~CRTSCTS;
			options->c_iflag &= ~(IXON|IXOFF|IXANY);
			break;
		case '1'://ON
			options->c_cflag |= CRTSCTS;
			options->c_iflag |= IXON|IXOFF|IXANY;
			break;
		default://OFF
			options->c_cflag &= ~CRTSCTS;
			options->c_iflag &= ~(IXON|IXOFF|IXANY);
			break;
	}
	/****************停止位选择****************/    
	if(stop_bits == 1)
	{     
		options->c_cflag &= ~CSTOPB;     
	}     
	else if(stop_bits ==2)
	{     
		options->c_cflag |= CSTOPB;     
	}
	else
	{
		options->c_cflag &= ~CSTOPB; 
	}
	/****************波特率选择****************/
    baud_rate = check_speed_parameter(speed);
	cfsetispeed(options, baud_rate);   
	cfsetospeed(options, baud_rate);
	return options;
}

/*************************************************************
* 功能：	设置回显
* 参数：	串口设备文件名
		echo：回显（0为关，1为开）
* 返回值：	无
**************************************************************/
void uart_set_echo(int uart_fd, int echo)
{
	struct termios options;
	
	tcgetattr(uart_fd, &options);	//保存串口属性
	if(echo == 0)
	{
		options.c_lflag &= ~ECHO;				/* 关回显 */
	}
	else if(echo == 1)
	{
		options.c_lflag |= ECHO;				/* 开回显 */
	}
	else
	{
		options.c_lflag &= ~ECHO;				/* 关回显 */
	}
	tcsetattr(uart_fd, TCSANOW, &options);	/* 设置串口属性 */
}

/*************************************************************
* 功能：	设置阻塞
* 参数：	串口设备文件名
		block：阻塞（0为不阻塞，1为阻塞）
* 返回值：	无
**************************************************************/
void uart_set_block(int uart_fd, int block)
{
	if(block == 0)
	{
		fcntl(uart_fd, F_SETFL, O_NONBLOCK);	//read时不阻塞
	}
	else if(block == 1)
	{
		fcntl(uart_fd, F_SETFL, 0);		//read时阻塞
	}
	else
	{
		fcntl(uart_fd, F_SETFL, O_NONBLOCK);	//read时不阻塞
	}
}

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
int uart_init(int uart_fd,
			  	int speed,
				int data_bits,
				int stop_bits,
				int check, 
				int flow_ctrl)
{
	struct termios *options;

	tcgetattr(uart_fd, &option_old);	//保存串口属性
	//uart_print_attr(&option_old);
	options = uart_set_attr(speed, data_bits, stop_bits, check, flow_ctrl, NULL);
	tcflush(uart_fd, TCIFLUSH);
	//uart_print_attr(options);
	tcsetattr(uart_fd, TCSANOW, options);	/* 设置串口属性 */
	if(options != &option_old){
		free(options);
	}
    return uart_fd;
}

/*************************************************************
* 功能：	串口反初始化程序
* 参数：	串口设备文件描述符
* 返回值：	无
**************************************************************/
void uart_uninit(int uart_fd)
{
	/*还原串口属性*/
	tcsetattr(uart_fd, TCSANOW, &option_old);
	/*关闭串口*/
	close(uart_fd);
}

/*************************************************************
* 功能：	串口发送字符串
* 参数：	uart_fd：串口设备文件描述符
			str：待发送的字符
* 返回值：	无
**************************************************************/
void uart_send_str(int uart_fd, char *str)
{
	int ret;
	
	ret = write(uart_fd, str, strlen(str));
	if(ret < 0)
	{
		printd(ERROR, "write");
	}
}

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
int uart_read_until_char(int uart_fd, char *buffer, int len, unsigned char until, int timeout_ms)
{
	unsigned char c = '\0';
	fd_set fds;
	struct timeval tv;
	int i = -1;
	int ret;

	memset(buffer, 0, len);
	for(i=0; i<len; i++){
		tv.tv_sec = 0;
		tv.tv_usec = timeout_ms*1000;
		FD_ZERO(&fds);
		FD_SET(uart_fd, &fds);
		ret = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
		if(ret < 0){
			printd(ERROR, "seclect error\n");
			return -1;
		}else if(ret > 0){
			if(FD_ISSET(uart_fd, &fds)){
				ret = read(uart_fd, &c, 1);
				if(ret < 0){
					printd(ERROR, "read error\n");
				}else if(ret == 0){
					printd(INFO, "end of file\n");
				}else{
					buffer[i] = c;
#if 0		
					printd(INFO, "i= %d\n", i);
					printd(INFO, "\t,[%#x], <%c>\n", c, c);
#endif				
					if(c == until){
						i++;
						break;
					}
				}
			}
		}else{
			//printd(ERROR, "read time out\n");
			return -2;
		}
	}
	return i;
}

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
int uart_read_until_time(int uart_fd, char *buffer, int len, int timeout_first, int timeout_interval)
{
	unsigned char c = '\0';
	fd_set fds;
	struct timeval tv;

	int i = -1;
	int ret;

	memset(buffer, 0, len);
	tv.tv_sec = 0;
	tv.tv_usec = timeout_first*1000;
	for(i=0; i<len; i++){
		FD_ZERO(&fds);
		FD_SET(uart_fd, &fds);
		ret = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
		if(ret < 0){
			printd(ERROR, "seclect error\n");
			return -1;
		}else if(ret > 0){
			if(FD_ISSET(uart_fd, &fds)){
				ret = read(uart_fd, &c, 1);
				if(ret < 0){
					printd(ERROR, "read error\n");
				}else if(ret == 0){
					printd(INFO, "end of file\n");
				}else{
					buffer[i] = c;
				}
			}
			tv.tv_sec = 0;
			tv.tv_usec = timeout_interval*1000;
		}else{
			if(i == 0){
				//printd(ERROR, "read time out\n");
				return -2;
			}else{
				return i;
			}
		}
	}
	return i;
}

