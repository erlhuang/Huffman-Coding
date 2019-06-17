//based off of darrell longs queue.h
# ifndef _QUEUE_H
# define _QUEUE_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

typedef treeNode items;

typedef struct queue
{
	uint32_t capacity;
	uint32_t tail;
	uint32_t head;
	uint32_t size;
	items **Q;
} queue;

queue *newQueue(uint32_t size); //constructor
void delQueue(queue *q); //deconstructor

bool queueEmpty(queue *q);
bool queueFull(queue *q);

bool enqueue(queue *q, items *i); 
bool dequeue(queue *q, items **i);

#endif
