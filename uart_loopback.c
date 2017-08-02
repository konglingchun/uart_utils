#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "uart_utils.h"

static int uart_fd = -1;
#define LOOPBACK_TEST_BUFF "loopback test"

int uart_loopback_test(char *file)
{
	char buffer[20] = "";
	
	uart_fd = uart_init(file, 9600, 8, 1, 'E', 0);

	write(uart_fd, LOOPBACK_TEST_BUFF, strlen(LOOPBACK_TEST_BUFF));
	sleep(1);
	read(uart_fd, buffer, sizeof(buffer));
	close(uart_fd);
	if(strncmp(buffer, LOOPBACK_TEST_BUFF, strlen(LOOPBACK_TEST_BUFF)) == 0){
		printf("uart %s loopback ok\n", file);
		return 0;
	}
	printf("uart %s loopback not ok\n", file);
	return -1;
}

