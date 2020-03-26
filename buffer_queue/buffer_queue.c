#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffer_queue.h"

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
    #define printd(fmt, args...)\
	do{	\
		printf(fmt, ##args);\
    }while(0)
#else
    #define printd(fmt, arg...)
#endif

/*
 * 队列初始化
 */
int _buffer_queue_init(_buffer_queue_t *handler, int capacity, int data_size)
{
	int ret = 0;
	
	handler->front = 0;
	handler->rear = 0;
	if(capacity == 0) {
		printf("capacity must greater than 0");
		ret = -1;
		goto error;
	}
	handler->capacity = capacity+1;
	handler->data = calloc(capacity, data_size);
	handler->data_size = data_size;
	if(handler->data == NULL) {
		perror("_buffer_queue_init: calloc");
		ret = -1;
	}
error:
	return ret;
}

/*
 * 清空队列
 */
void _buffer_queue_empty(_buffer_queue_t *handler)
{
	handler->front = 0;
	handler->rear = 0;
}

/*
 * 队列反初始化
 */
void _buffer_queue_uninit(_buffer_queue_t *handler)
{
	handler->front = 0;
	handler->rear = 0;
	handler->capacity = 0;
	free(handler->data);
	handler->data = NULL;
}

/*
 * 向队列添加元素
 */
static int _buffer_queue_enqueue_without_flush(_buffer_queue_t *handler, void *data)
{
	if(((handler->rear + 1) % handler->capacity == handler->front))
		return -1;
	memcpy((unsigned char *)handler->data + handler->rear * handler->data_size, data, handler->data_size);
	handler->rear = (handler->rear + 1) % handler->capacity;
	printd("front = %d, rear = %d\r\n", handler->front, handler->rear);
	return 0;
}

/*
 * 向队列添加元素，若队列已满会清出队头的元素
 */
int _buffer_queue_enqueue(_buffer_queue_t *handler, void *data)
{
	int size = 0;
	int ret;

	size = _buffer_queue_size(handler);
	if(size == (handler->capacity - 1)) {
		ret = _buffer_queue_dequeue(handler, NULL);
		if(ret != 0) {
			goto error;
		}
		ret = _buffer_queue_enqueue_without_flush(handler, data);
		if(ret != 0) {
			goto error;
		}
	} else {
		ret = _buffer_queue_enqueue_without_flush(handler, data);
		if(ret != 0) {
			goto error;
		}
	}
error:	
	return ret;
}

/*
 * 向队列添加多个元素，若队列已满会清出队头的元素
 */
int _buffer_queue_enqueue_multi(_buffer_queue_t *handler, void *data_arry, int arry_size)
{
	int ret = 0;
	int i;
	
	for(i=0;i<arry_size;i++) {
		ret = _buffer_queue_enqueue(handler, (unsigned char *)data_arry + i*handler->data_size);
		if(ret) {
			goto error;
		}
	}
error:
	return ret;
}

/*
 * 清出队头的元素
 */
void _buffer_queue_flush(_buffer_queue_t *handler, int size)
{
	int queue_size;
	
	queue_size = _buffer_queue_size(handler);
	if(queue_size >= size) {
		handler->front = (handler->front + size) % handler->capacity;
	} else {
		handler->front = (handler->front + queue_size) % handler->capacity;
	}
}

/*
 * 清出队尾的元素
 */
void _buffer_queue_delete(_buffer_queue_t *handler)
{
	int size;
	int size_enqueued;
	int i;
	
	size = _buffer_queue_size(handler);
	if(size){
		handler->rear = (handler->rear - 1 + handler->capacity)%handler->capacity;
	}
}

/*
 * 从队列取出元素
 */
int _buffer_queue_dequeue(_buffer_queue_t *handler, void *data)
{
	if(handler->front == handler->rear) 
		return -1;
	if(data != NULL) {
		memcpy(data, (unsigned char *)handler->data + handler->front * handler->data_size, handler->data_size);
	}
	printd("front = %d, rear = %d\r\n", (handler->front + 1) % handler->capacity, handler->rear);
	handler->front = (handler->front + 1) % handler->capacity;
	return 0; 
}

/*
 * 从队列取出多个元素
 */
int _buffer_queue_dequeue_multi(_buffer_queue_t *handler, void *data_arry, int arry_size)
{
	int queue_size;
	int size_dequeued;
	int i;
	
	if(data_arry != NULL) {
		queue_size = _buffer_queue_size(handler);
		if(queue_size <= arry_size) {
			size_dequeued = queue_size;
		} else {//queue_size > arry_size
			size_dequeued = arry_size;
		}
		for(i=0;i<size_dequeued;i++) {
			_buffer_queue_dequeue(handler, (unsigned char *)data_arry + i);
		}
	}
	printd("size_dequeued = %d, front = %d, rear = %d\r\n", size_dequeued, handler->front, handler->rear);
	return size_dequeued; 
}

/*
 * 队列元素个数
 */
int _buffer_queue_size(_buffer_queue_t *handler)
{
	return (handler->rear - handler->front + handler->capacity)% handler->capacity;
}

/*
 * 队列剩余存放元素的空间的大个数
 */
int _buffer_queue_size_left(_buffer_queue_t *handler)
{
	int capacity_left = 0;
	
	capacity_left = handler->capacity - 1 - ((handler->rear - handler->front + handler->capacity)% handler->capacity);
	return capacity_left;
}

/*
 * The index of the first element is 0
 */
int _buffer_queue_ergodic_buffer(_buffer_queue_t *handler, void *data_arry, int start, int length)
{
	unsigned int ergodic_ptr = 0;
	int queue_size = 0;
	unsigned char *buffer_temp;
	int queue_readable_size = 0;
	int queue_readable_temp = 0;

	queue_size = _buffer_queue_size(handler);
	if(queue_size < (start+1)){
		return -1;
	}
	queue_readable_size = queue_size - start;
	if(queue_readable_size >= length){
		queue_readable_size = length;
	} else {//do nothing
		//queue_readable_size = queue_readable_size;
	}
	queue_readable_temp = queue_readable_size;
	ergodic_ptr = (handler->front + start)% handler->capacity;
	buffer_temp = data_arry;
	while((ergodic_ptr != handler->rear)
		 && (queue_readable_temp != 0)) {
		memcpy(buffer_temp, (unsigned char *)handler->data + ergodic_ptr * handler->data_size, handler->data_size);
		buffer_temp += handler->data_size;
		ergodic_ptr = (ergodic_ptr + 1) % handler->capacity;
		queue_readable_temp--;
	}
	return queue_readable_size;
}
