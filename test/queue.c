#include <stdio.h>
#include <string.h>

#include "buffer_queue.h"
#include "buffer_utils.h"

void queue_test_0(void)
{
	_buffer_queue_t handler;
	char data_arry[128] = "";
	char data_enqueue[] = "abcdef";
	char data_dequeue[128] = "";
	int data_size = 0;

	_buffer_queue_init(&handler, 10, QUEUE_CHAR);
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	
	_buffer_queue_enqueue(&handler, "0");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "1");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "2");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "3");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "4");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "5");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "6");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "7");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "8");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "9");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	_buffer_queue_enqueue(&handler, "A");
	printf("queue_size left = %d\r\n", _buffer_queue_size_left(&handler));
	
	//test 0
	data_size = _buffer_queue_size(&handler);
	memset(data_arry, 0, sizeof(data_arry));
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 0: ergodic_buffer", data_arry, data_size);
	
	
	//test 1
	_buffer_queue_enqueue(&handler, "B");
	data_size = _buffer_queue_size(&handler);
	memset(data_arry, 0, sizeof(data_arry));
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 1-1: queue_enqueue", data_arry, data_size);

	_buffer_queue_enqueue(&handler, "C");
	data_size = _buffer_queue_size(&handler);
	memset(data_arry, 0, sizeof(data_arry));
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 1-2: queue_enqueue", data_arry, data_size);
		
	//test 2
	_buffer_queue_flush(&handler, 4);
	data_size = _buffer_queue_size(&handler);
	memset(data_arry, 0, sizeof(data_arry));
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 2: ququq_flush", data_arry, data_size);
	
	//test 3
	_buffer_queue_enqueue_multi(&handler, data_enqueue, strlen(data_enqueue));
	data_size = _buffer_queue_size(&handler);
	memset(data_arry, 0, sizeof(data_arry));
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 3: enqueue_multi", data_arry, data_size);
	
	//test 4
	_buffer_queue_dequeue_multi(&handler, data_dequeue, 5);
	data_size = _buffer_queue_size(&handler);
	memset(data_arry, 0, sizeof(data_arry));
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 4-1: dequeue_multi", data_dequeue, 5);
	print_buffer("test 4-2: ergodic_buffer", data_arry, data_size);
	
	_buffer_queue_uninit(&handler);
}

void queue_test_1(void)
{
	_buffer_queue_t handler;
	typedef struct{
		double x;
		double y;
	}coordinate_t;
	coordinate_t a;
	coordinate_t data_arry[5] = {0};
	int data_size = 0;
	int i;

	printf("save struct data in queue\n");
	_buffer_queue_init(&handler, 5, sizeof(coordinate_t));
	printf("queue_size left = %d\n", _buffer_queue_size_left(&handler));

	a.x = 0; a.y = 0;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 1; a.y = 1;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 2; a.y = 2;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 3; a.y = 3;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 4; a.y = 4;
	_buffer_queue_enqueue(&handler, &a);	
	a.x = 5; a.y = 5;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 6; a.y = 6;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 7; a.y = 7;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 8; a.y = 8;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 9; a.y = 9;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 10; a.y = 10;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 11; a.y = 11;
	_buffer_queue_enqueue(&handler, &a);
	a.x = 12; a.y = 12;
	_buffer_queue_enqueue(&handler, &a);
	
	data_size = _buffer_queue_size(&handler);

	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	printf("ergodic queue\n");
	for(i=0;i<data_size;i++){
		printf("i=%d, x=%.0f,y=%.0f\n", i, data_arry[i].x, data_arry[i].y);
	}

	_buffer_queue_uninit(&handler);
}

int main(int argc, char *argv[])
{
	queue_test_0();
	queue_test_1();
	return 0;
}
