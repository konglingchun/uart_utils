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
		uart_fd = uart_init(argv[1], 115200, 8, 1, 'N', 0);
		while(1){
			ret = uart_read_until_time(uart_fd, buffer, sizeof(buffer), 5000, 10);
			//ret = read(uart_fd, buffer, sizeof(buffer));
			if(ret > 0){
				print_buffer("receive buffer", buffer, ret);
				print_buffer_char_index("receive char", buffer, ret);
				print_buffer_hex_index("receive hex", buffer, ret);
			}
		}
	}else{
		printf("usage: ./main /dev/ttyUSB*\n");
	}
    return 0;
}
