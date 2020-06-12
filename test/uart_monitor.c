#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "uart_utils.h"
#include "uart_loopback.h"
#include "buffer_utils.h"

void *uart_read_thread(void *arg)
{
	char buffer[256] = "";
	int uart_fd = *((int *)arg);
	int ret;

	while(1){
		ret = uart_read_until_time(uart_fd, buffer, sizeof(buffer), 1000, 10);
		//ret = read(uart_fd, buffer, sizeof(buffer));
		if(ret > 0){
			print_buffer("receive buffer", buffer, ret);
			print_buffer_char_index("receive char", buffer, ret);
			print_buffer_hex_index("receive hex", buffer, ret);
		}
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int uart_fd;
	char buffer[256] = "";
	int ret;
	pthread_t thread_id;

	if(argc == 2
		&& strstr(argv[1], "/dev/tty")){
		uart_fd = uart_open(argv[1]);
		uart_init(uart_fd, 115200, 8, 1, 'N', 0);
		pthread_create(&thread_id, NULL, uart_read_thread, &uart_fd);
		while(1){
			fgets(buffer, sizeof(buffer), stdin);
			buffer[strlen(buffer) - 1] = '\r';
			buffer[strlen(buffer)] = '\n';
			print_buffer_char("send char", buffer, strlen(buffer));
			print_buffer_hex("send hex", buffer, strlen(buffer));
			write(uart_fd, buffer, strlen(buffer));
		}
	}else{
		printf("usage: ./main /dev/tty*\n");
	}
    return 0;
}
