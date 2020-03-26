#ifndef _BUFFER_QUEUE_H_
#define _BUFFER_QUEUE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define QUEUE_CHAR sizeof(char)
#define QUEUE_INT sizeof(int)
#define QUEUE_LONG sizeof(long)
#define QUEUE_FLOAT sizeof(float)
#define QUEUE_DOUBLE sizeof(double)

typedef struct {
	unsigned int front;
	unsigned int rear;
	void *data;// queue data
	int data_size;
	unsigned int capacity;//max data in queue
}_buffer_queue_t;

/*
 * 队列初始化
 */
int _buffer_queue_init(_buffer_queue_t *handler, int capacity, int data_size);

/*
 * 清空队列
 */
void _buffer_queue_empty(_buffer_queue_t *handler);

/*
 * 队列反初始化
 */
void _buffer_queue_uninit(_buffer_queue_t *handler);


/*
 * 向队列添加元素，若队列已满会清出队头的元素
 */
int _buffer_queue_enqueue(_buffer_queue_t *handler, void *data);

/*
 * 向队列添加多个元素，若队列已满会清出队头的元素
 */
int _buffer_queue_enqueue_multi(_buffer_queue_t *handler, void *data_arry, int arry_size);

/*
 * 清除队头的元素
 */
void _buffer_queue_flush(_buffer_queue_t *handler, int size);

/*
 * 从队列取出元素
 */
int _buffer_queue_dequeue(_buffer_queue_t *handler, void *data);

/*
 * 从队列取出多个元素
 */
int _buffer_queue_dequeue_multi(_buffer_queue_t *handler, void *data_arry, int arry_size);

/*
 * 队列元素个数
 */
int _buffer_queue_size(_buffer_queue_t *handler);

/*
 * 队列剩余存放元素的空间的个数
 */
int _buffer_queue_size_left(_buffer_queue_t *handler);

/*
 * The index of the first element is 0
 */
int _buffer_queue_ergodic_buffer(_buffer_queue_t *handler, void *data_arry, int start, int length);


#ifdef  __cplusplus
}
#endif

#endif
