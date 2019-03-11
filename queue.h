//based off of darrell longs queue.h
//oliver mulcahy (opm@ucsc.edu)
# ifndef _QUEUE_H
# define _QUEUE_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

typedef treeNode items;

typedef struct queue
{
	uint32_t size;
	uint32_t tail;
	uint32_t head;
	items *Q;
} queue;

queue *newQueue(uint32_t size); //constructor
void delQueue(queue *q); //deconstructor

bool queueEmpty(queue *q);
bool queueFull(queue *q);

bool enqueue(queue *q, items i); //add
bool dequeue(queue *q, items *i); //remove (from top so it's smallest)

#endif
