#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "uart_utils.h"
#include "uart_loopback.h"
#include "buffer_utils.h"
#include "debug_print.h"

void clear_screen(void)
{
    printf("\033[2J");
    fflush(stdout);
}

void clear_to_end(void)
{
    printf("\033[K"); 
    fflush(stdout);
}

void cusor_moveto(int x, int y)
{
    printf("\033[%d;%dH",y,x);
    fflush(stdout);
}

void cusor_moveto_clear(int x, int y)
{
    cusor_moveto(x, y);
	clear_to_end();
	cusor_moveto(x, y);
}

int main(int argc, char *argv[])
{
	int uart_fd;
	int receive_length;
	char receive_buffer[1024] = "";
	char *result = NULL;

	uart_fd = uart_init("/dev/ttyUSB0", 9600, 8, 1, 'N', 0);
	clear_screen();
	while(1){
		receive_length = uart_read_until_char(uart_fd, receive_buffer, sizeof(receive_buffer), '\n', 50);
		if(receive_length > 0){
			//print_buffer("", receive_buffer, receive_length);
			//print_buffer_hex("", receive_buffer, receive_length);
			if(strstr(receive_buffer, " Xout") != NULL
				&& strstr(receive_buffer, " Yout") != NULL){
				result = strstr(receive_buffer, " Xout");
				cusor_moveto_clear(1, 1);
				printd(INFO, "%s", result);
			}else if(strstr(receive_buffer, " angle") != NULL){
				result = strstr(receive_buffer, " angle");
				cusor_moveto_clear(1, 2);
				printd(INFO, "%s", result);
			}else if(strstr(receive_buffer, " Pitch") != NULL
				&& strstr(receive_buffer, " Yoll") != NULL){
				result = strstr(receive_buffer, " Pitch");
				cusor_moveto_clear(1, 3);
				printd(INFO, "%s", result);
			}
		}
	}
    return 0;
}
