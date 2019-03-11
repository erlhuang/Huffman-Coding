# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <time.h>
# include "queue.h"
//Max PQ implemented through heaps

queue *newQueue(uint32_t size)
{
	queue *q = malloc(sizeof(queue));
	q -> size = size;
	q -> head = 0;
	q -> tail = 0; //bottom of list
	q -> Q = calloc(size, sizeof(items));
	return q;
}
//returns the parent index of index i
uint32_t parent(uint32_t i){
	return ((i-1)/2);
}

uint32_t left(uint32_t i){
	return ((2*i) + 1);
}

uint32_t right(uint32_t i){
	return ((2*u) + 2);
}
void delQueue(queue *q)
{
	free(q -> Q);
	free(q);
	return;
}

bool queueEmpty(queue *q)
{
	//if no queue or the tail is at the beginning
	if ((q == (queue *) 0) || (q -> tail == 0))
	{
		return true;
	}
	else return false;
}
bool queueFull(queue *q)
{
	//tail is at the end
	if (q -> tail == (q->size - 1))
	{
		return true;
	}
	else return false;
}

void maxHeapify(queue *q, uint32_t i){
	uint32_t l = left(i);
	uint32_t r = right(i);
	int biggest = i;
	if(l < q->tail && q->Q[l] > q->Q[i]){
		biggest = l;
	}
	if(r < q->tail && q->Q[r] > q->Q[biggest]){
		biggest = r;
	}
	if(biggest != i){
		items temp = q->Q[i];
		q->Q[i] = q->Q[biggest];
		q->Q[biggest] = temp;
		maxHeapify(*q, biggest);
	}
}

bool dequeue(queue *q, items *z)
{
	if (queueEmpty(q))
	{
		return false;
	}
	else if (q -> tail = 1){
	 	*z = q -> Q[0];
		q -> tail--;
		return true;
	}
	else{
		items root = q -> Q[0];
		q->tail = q->tail - 1;
		q -> Q[0] = q -> Q[x];
		maxHeapify(*q, 0);

}

bool enqueue(queue *q, items i){
	if (queueFull(q)){
		return false;
	}
	else{
		q -> Q [q -> tail] = i;
		uint32_t x = q -> tail;
		while( x != 0 && q -> Q[parent(x)] < q->Q[x]){
			items temp = q -> Q[parent(x)]; //swap them
			q -> Q[parent(x)] = q -> Q[x];
			q -> Q[x] = temp;
			x = parent(x);
		}
		q -> tail = q -> tail + 1;
		return true;
	}
}
