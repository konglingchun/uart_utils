#include <stdio.h>

void print_buffer_hex(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[34;1m");
	fflush(stdout);
	printf("%s{", pre);
	for(i=0;i<size;i++) {
		printf("%#02hhx,", (unsigned char)buffer[i]);
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer_hex_index(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[34;1m");
	fflush(stdout);
	printf("%s{", pre);
	for(i=0;i<size;i++) {
		printf("[%d]=", i);
		printf("%#02hhx,", (unsigned char)buffer[i]);
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer(char *pre, char *buffer, int size)
{
	printf("\e[31;1m");
	fflush(stdout);
	printf("%s{%s}\n", pre, buffer);
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer_char(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[32;1m");
	fflush(stdout);
	printf("%s{", pre);
	for(i=0;i<size;i++) {
		if(buffer[i] == '\r'){
			printf("'\\r',");
		}else if(buffer[i] == '\n'){
			printf("'\\n',");
		}else{
			printf("%c,", (unsigned char)buffer[i]);
		}
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}


