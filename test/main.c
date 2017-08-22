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

	uart_fd = uart_init("/dev/ttyUSB0", 9600, 8, 1, 'N', 0);
	while(1){		
		receive_length = uart_read_until(uart_fd, receive_buffer, sizeof(receive_buffer), '\n', 50);
		if(receive_length > 0){
			print_buffer("", receive_buffer, receive_length);
			print_buffer_hex("", receive_buffer, receive_length);
		}
	}
    return 0;
}
