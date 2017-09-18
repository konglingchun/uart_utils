#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "uart_utils.h"
#include "buffer_utils.h"
#include "buffer_queue.h"
#include "checksum8.h"

//#define RUF_MASK 0x80
//x^16 + x^12 + x^5 + 1
//
#define POLYNOMIAL 0x8408
#define PRESET_VALUE 0xFFFF
#define CHECK_VALUE 0xF0B8
#define CALC_CRC 0x1
#define CHECK_CRC 0x0
//
unsigned rfid_crc_calc(unsigned char *data_byte, unsigned int byte_len)
{
	unsigned int current_crc_value;
	unsigned int i,j;
	current_crc_value = PRESET_VALUE;
	
	for(i=0; i<byte_len; i++)
	{
		current_crc_value = current_crc_value^data_byte[i];
		for(j=0; j<8; j++)
		{
			if(current_crc_value&0x0001)
			{
				current_crc_value = (current_crc_value>>1)^POLYNOMIAL;
			}
			else
			{
				current_crc_value = (current_crc_value>>1);
			}
		}
	}
	current_crc_value = ~current_crc_value;
	return current_crc_value;
}

int rfid_read_uid_request(int uart_fd, uint16_t addr_src, uint16_t addr_dest)
{
	unsigned char request[12] = {0x7E, 0x55, 0x09, 0x00, 0x00, 0xAA, 0xAA, 0x11, 0x00, 0x00};
	uint16_t crc = 0;
	int ret = 0;
		
	request[3] = addr_src&0xff;
	request[4] = addr_src>>8;
	request[5] = addr_dest&0xff;
	request[6] = addr_dest>>8;
	crc = rfid_crc_calc(request+2, request[2]-1);
	request[10] = crc&0xff;
	request[11] = crc>>8;
	print_buffer_hex("rfid_read_uid_request", request, request[2]+3);
	ret = write(uart_fd, request, request[2]+3);
	if(ret < 0)
	{
		printf("rfid_read_uid_request write error\n");
	}
	usleep(50*1000);//cmd request must wait 50 ms at least
	return ret;
}

int rfid_read_data_request(int uart_fd, uint16_t addr_src, uint16_t addr_dest, unsigned long long uid)
{
	unsigned char request[21] = {0x7E, 0x55, 0x12, 0x00, 0x00, 0xAA, 0xAA, 0x22, 0x00, 0x16, 0xF1, 0xF7, 0x2D, 0x00, 0x01, 0x04, 0xE0, 0x00, 0x02};
	uint16_t crc = 0;
	int ret = 0;
		
	request[3] = addr_src&0xff;
	request[4] = addr_src>>8;
	request[5] = addr_dest&0xff;
	request[6] = addr_dest>>8;
	request[9] = (uid>>0)&0xff;
	request[10] = (uid>>1*8)&0xff;
	request[11] = (uid>>2*8)&0xff;
	request[12] = (uid>>3*8)&0xff;
	request[13] = (uid>>4*8)&0xff;
	request[14] = (uid>>5*8)&0xff;
	request[15] = (uid>>6*8)&0xff;
	request[16] = (uid>>7*8)&0xff;
	crc = rfid_crc_calc(request+2, request[2]-1);
	request[19] = crc&0xff;
	request[20] = crc>>8;
	print_buffer_hex("rfid_read_data_request", request, request[2]+3);
	ret = write(uart_fd, request, request[2]+3);
	if(ret < 0)
	{
		printf("rfid_read_data_request write error\n");
	}
	return ret;
}


int rfid_read_uid_response(int uart_fd, unsigned char *buffer, unsigned int len)
{
	int receive_length;

	receive_length = uart_read_until_time(uart_fd, buffer, len, 200, 10);
	if(receive_length > 0){
		print_buffer_hex("rfid_read_uid_response", buffer, receive_length);
	}
	return receive_length;
}

int rfid_read_data_response(int uart_fd, unsigned char *buffer, unsigned int len)
{
	int receive_length;

	receive_length = uart_read_until_time(uart_fd, buffer, len, 200, 10);
	if(receive_length > 0){
		print_buffer_hex("rfid_read_data_response", buffer, receive_length);
	}
	return receive_length;
}


int rfid_read_uid_analysis(unsigned char *buffer, unsigned int len, unsigned long long *uid)
{
	long ret = -1;

	print_buffer_hex_index("rfid_read_uid_analysis", buffer, len);
	if(buffer[0] = 0x7E 
		&& buffer[1] == 0x55
		&& buffer[2] >= 0x14
		&& buffer[7] == 0x1f
		&& buffer[8] == 0x11
		&& len >= 23){
		if(uid != NULL){
			*uid = (((unsigned long long)buffer[17])<<(7*8))
				|(((unsigned long long)buffer[16])<<(6*8))
				|(((unsigned long long)buffer[15])<<(5*8))
				|(((unsigned long long)buffer[14])<<(4*8))
				|(((unsigned long long)buffer[13])<<(3*8))
				|(((unsigned long long)buffer[12])<<(2*8))
				|(((unsigned long long)buffer[11])<<(1*8))
				|(((unsigned long long)buffer[10])<<(0*8));
			ret = 0;
		}
	}else{
		printf("Mismatch with protocol or other error\n");
	}
	return ret;
}

int rfid_read_data_analysis(unsigned char *buffer, unsigned int len, unsigned long long *data)
{
	long ret = -1;

	print_buffer_hex_index("rfid_read_data_analysis", buffer, len);
	if(buffer[0] = 0x7E 
		&& buffer[1] == 0x55
		&& buffer[2] >= 0x13
		&& buffer[7] == 0x1f
		&& buffer[8] == 0x22
		&& buffer[18] == 0//judge ok or error
		&& buffer[19] == 0//judge ok or error
		&& len >= 22){
		if(data != NULL){
			*data = (((unsigned long long)buffer[10])<<(7*8))
				|(((unsigned long long)buffer[11])<<(6*8))
				|(((unsigned long long)buffer[12])<<(5*8))
				|(((unsigned long long)buffer[13])<<(4*8))
				|(((unsigned long long)buffer[14])<<(3*8))
				|(((unsigned long long)buffer[15])<<(2*8))
				|(((unsigned long long)buffer[16])<<(1*8))
				|(((unsigned long long)buffer[17])<<(0*8));
			ret = 0;
		}
	}else{
		printf("Mismatch with protocol or other error\n");
	}
	return ret;
}

void rfid_read_uid_demo(int uart_fd, unsigned long long *uid)
{
	int receive_length;
	char receive_buffer[1024] = "";
	int ret;
	
uid_retry:
	rfid_read_uid_request(uart_fd, 0x00, 0xabcd);
	receive_length = rfid_read_uid_response(uart_fd, receive_buffer, sizeof(receive_buffer));
	ret = rfid_read_uid_analysis(receive_buffer, receive_length, uid);
	if(ret < 0){
		usleep(100*1000);
		goto uid_retry;
	}
	printf("%llX\n", *uid);
	usleep(50*1000);
}

void rfid_read_data_demo(int uart_fd, unsigned long long uid)
{
	int receive_length;
	char receive_buffer[1024] = "";
	unsigned long long data = 0;
	int ret;
	
data_retry:
	rfid_read_data_request(uart_fd, 0x00, 0xabcd, uid);
	receive_length = rfid_read_data_response(uart_fd, receive_buffer, sizeof(receive_buffer));
	ret = rfid_read_data_analysis(receive_buffer, receive_length, &data);
	if(ret < 0){
		usleep(100*1000);
		goto data_retry;
	}
	printf("%#llX\n", data);
	usleep(50*1000);
}

void rfid_test(void)
{
	int uart_fd;
	unsigned long long uid;
	
	uart_fd = uart_init("/dev/ttyUSB0", 38400, 8, 1, 'N', 0);
	while(1){
		rfid_read_uid_demo(uart_fd, &uid);
	}
	while(1){		
		rfid_read_data_demo(uart_fd, uid);
	}
}

int main(int argc, char *argv[])
{
	rfid_test();
    return 0;
}

