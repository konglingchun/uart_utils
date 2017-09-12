#include <stdio.h>
#include <string.h>

#include "buffer_queue.h"
#include "buffer_utils.h"


int main(int argc, char *argv[])
{
	_buffer_queue_t handler;
	char data_arry[256] = "";
	char data_enqueue[] = "bcdef";
	char data_dequeue[256] = "";
	int data_size = 0;
	
	_buffer_queue_init(&handler, 10, QUEUE_CHAR);
	printf("queue_size left = %d\n", _buffer_queue_size_left(&handler));
	
	_buffer_queue_enqueue(&handler, "0");
	_buffer_queue_enqueue(&handler, "1");
	_buffer_queue_enqueue(&handler, "2");
	_buffer_queue_enqueue(&handler, "3");
	_buffer_queue_enqueue(&handler, "4");
	_buffer_queue_enqueue(&handler, "5");
	_buffer_queue_enqueue(&handler, "6");
	_buffer_queue_enqueue(&handler, "7");
	_buffer_queue_enqueue(&handler, "8");
	_buffer_queue_enqueue(&handler, "9");
	_buffer_queue_enqueue(&handler, "a");
	
	//test 0
	data_size = _buffer_queue_size(&handler);
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 0: ergodic_buffer ", data_arry, data_size);
	
	
	//test 1
	_buffer_queue_enqueue_flush(&handler, "a");
	data_size = _buffer_queue_size(&handler);
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 1: enqueue_flush ", data_arry, data_size);
	
	//test 2
	_buffer_queue_flush(&handler, 4);
	data_size = _buffer_queue_size(&handler);
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 2: ququq_flush ", data_arry, data_size);
	
	//test 3
	_buffer_queue_enqueue_multi(&handler, data_enqueue, strlen(data_enqueue));
	data_size = _buffer_queue_size(&handler);
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 3: enqueue_multi ", data_arry, data_size);
	
	//test 4
	_buffer_queue_dequeue_multi(&handler, data_dequeue, 5);
	data_size = _buffer_queue_size(&handler);
	_buffer_queue_ergodic_buffer(&handler, data_arry, 0, data_size);
	print_buffer("test 4-1: dequeue_multi ", data_dequeue, 5);
	print_buffer("test 4-2: ergodic_buffer ", data_arry, data_size);
	
	_buffer_queue_uninit(&handler);
	return 0;
}
