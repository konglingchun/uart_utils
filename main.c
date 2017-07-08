#include <stdio.h>
#include <stdlib.h>
#include "uart_utils.h"

void print_buffer(char *pre, char *buffer, int size)
{
	int i;
	
	for(i=0;i<size;i++) {
		printf("%#02hhx\n", buffer[i]);
	}
}

int main(int argc, char *argv[])
{
	int uart_fd;
	int receive_length;
	char receive_buffer[1024] = "";
	
	uart_fd = uart_init("/dev/ttyUSB1", 9600, 8, 1, 'E', 0);
	while(1){		
		receive_length = read(uart_fd, receive_buffer, sizeof(receive_buffer));
		if(receive_length < 0) {
			perror("read");
		} else if(receive_length > 0){
			print_buffer("", receive_buffer, receive_length);
		}
	}
    return 0;
}
