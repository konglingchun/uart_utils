#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "uart_utils.h"
#include "debug_print.h"

static int uart_fd = -1;
#define LOOPBACK_TEST_BUFF "loopback test\n"

int uart_loopback_test(char *file)
{
	char buffer[20] = "";

	uart_fd = uart_open(file);
    uart_init(uart_fd, 9600, 8, 1, 'E', 0);
	write(uart_fd, LOOPBACK_TEST_BUFF, strlen(LOOPBACK_TEST_BUFF));
	uart_read_until_char(uart_fd, buffer, sizeof(buffer), '\n', 1000);
	uart_uninit(uart_fd);
	if(strncmp(buffer, LOOPBACK_TEST_BUFF, strlen(LOOPBACK_TEST_BUFF)) == 0){
		printd(INFO, "%s loopback is ok\n", file);
		return 0;
	}
	printd(INFO, "%s loopback is not ok\n", file);
	return -1;
}

