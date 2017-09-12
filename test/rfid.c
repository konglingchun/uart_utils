#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uart_utils.h"
#include "buffer_utils.h"
#include "buffer_queue.h"
#include "checksum8.h"

enum{
	OK,
	NOTOK
};

int check_integrity(void *data)
{
	unsigned char *data_ptr = data;
	
	if(data_ptr[0] == 0x7f) {
		if(checksum8_xor(data_ptr, 1, data_ptr[1]) == data_ptr[data_ptr[1]+1]) {
			return OK;
		}
	} else {
		return NOTOK;
	}
	return OK;
}

int rfid_find_head(unsigned char *buffer, int size)
{
	int i;
	int head = -1;
	
	if(size < 5){
		return -1;
	}
	for(i=0;i<(size - 1);i++) {
		if((buffer[i] == 0x7f)
		  &&(buffer[i+1] != 0x7f)){
			head = i;
			break;
		}
	}
	return head;
}

int rfid_get_command_length(unsigned char *buffer, int head)
{
	return buffer[head+1];
}

int rfid_get_char_length(unsigned char *buffer, int length, int size)
{

	return 0;
}

void print_buffer(char *pre, unsigned char *buffer, int size)
{
	int i;
	
	printf("%s", pre);
	for(i=0;i<size;i++) {
		printf("%02x,", buffer[i]);
	}
	printf("\n");
}

void rfid_process(_buffer_queue_t *handler, unsigned char *buffer, int size)
{
	int head, length;

	//print_buffer("src:", buffer, size);
	head = rfid_find_head(buffer, size);
	if(head >= 0){
		length = rfid_get_command_length(buffer, head);
		//printf("length = %d, size = %d\n", length, size-head);
		if((length+2) <= size){
			printf("device:%d\n", (buffer+head)[2]);
			print_buffer("\tdata:", buffer+head, size-head);
			//rfid_get_char_length(buffer+head, length, size-head);
			_buffer_queue_empty(handler);
		} else {
			//print_buffer("dest2:", buffer+head, size-head);
		}
	}
}

int main(int argc, char *argv[])
{
	int uart_fd;
	unsigned char receive_buffer[256] = "";
	unsigned char process_buffer[256] = "";
	int receive_length = 0;
	_buffer_queue_t handler;
	
	/*打开并初始化串口*/
	uart_fd = uart_init("/dev/ttyUSB0", 115200, 8, 1, 'N', 0);
	_buffer_queue_init(&handler, 256);
	
	/*串口发送信息*/
	//write(uart_fd, "this is test\n", strlen("this is test\n"));	
	
	/*串口接收信息*/
	while(1) {
		fd_set fds;
		int ret;
		
		FD_ZERO(&fds);
		FD_SET(uart_fd, &fds);
		ret = select(FD_SETSIZE, &fds, NULL, NULL, NULL);
		if(ret < 0){
			perror("seclect");
			return -1;
		}else if(ret > 0){
			memset(receive_buffer, 0, sizeof(receive_buffer));
			receive_length = read(uart_fd, receive_buffer, sizeof(receive_buffer));
			if(receive_length < 0){
				perror("read");
			} else {
				ret = _buffer_queue_enqueue_multi(&handler, receive_buffer, receive_length);
				if(ret != receive_length){
					printf("queue full\n");
					_buffer_queue_flush(&handler, 256);
				}
			}
		}else{
			return -1;
		}
		ret = _buffer_queue_ergodic_buffer(&handler, process_buffer, 0, sizeof(process_buffer));
		rfid_process(&handler, process_buffer, ret);
	}
	uart_uninit(uart_fd);
	return 0;
}

