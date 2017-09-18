#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "uart_utils.h"
#include "uart_loopback.h"
#include "buffer_utils.h"


int main(int argc, char *argv[])
{
	int uart_fd;
	char buffer[256] = "";
	int ret;

	if(argc == 2
		&& strstr(argv[1], "/dev/tty")){
		uart_fd = uart_init(argv[1], 38400, 8, 1, 'N', 0);
		while(1){
			ret = uart_read_until_time(uart_fd, buffer, sizeof(buffer), 500, 10);
			if(ret > 0){
				print_buffer_hex("receive monitor", buffer, ret);
			}
		}
	}else{
		printf("usage: ./main /dev/ttyUSB*\n");
	}
    return 0;
}
