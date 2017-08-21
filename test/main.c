#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uart_utils.h"
#include "uart_loopback.h"
#include "buffer_utils.h"


int main(int argc, char *argv[])
{
	int uart_fd;
	int receive_length;
	char receive_buffer[1024] = "";

	uart_fd = uart_init("/dev/ttyUSB0", 9600, 8, 1, 'E', 0);
	while(1){		
		receive_length = read(uart_fd, receive_buffer, sizeof(receive_buffer));
		if(receive_length < 0) {
			perror("read");
		} else if(receive_length > 0){
			print_buffer_hex("", receive_buffer, receive_length);
		}
	}
    return 0;
}
