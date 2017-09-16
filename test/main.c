#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uart_utils.h"
#include "uart_loopback.h"
#include "buffer_utils.h"


int main(int argc, char *argv[])
{
	int uart_fd;
	char buffer[256] = "";
	int ret;

	uart_fd = uart_init("/dev/ttyUSB1", 38400, 8, 1, 'N', 0);
	while(1){
		ret = uart_read_until_time(uart_fd, buffer, sizeof(buffer), 500, 10);
		if(ret > 0){
			print_buffer_hex("receive monitor", buffer, ret);
		}
	}
    return 0;
}
