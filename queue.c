# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <time.h>
# include "queue.h"
# include <stdio.h>
# define SWAP(x,y) { treeNode *temp = x; x = y; y = temp; }

//code adapted from darrell long's example
queue *newQueue(uint32_t size)
{
	queue *j = malloc(sizeof(queue));
	j->capacity = size;
	j->head = 0;
	j->tail = 0;
	j->Q = calloc(size, sizeof(items));
	for(uint32_t i = 0; i < j->capacity; i++)
	{
		j->Q[i] = NULL;
	}
	j->size = 0;
	return j;
}

void delQueue(queue *q)
{
	for(uint32_t i = 0; i < q->capacity; i++ )
	{
		if(q->Q[i] != NULL)
		{
			free(q->Q[i]);
		}
	}
	free(q->Q);
	free(q);
}

bool queueEmpty(queue *q)
{
	return q->tail == q->head;
}

bool queueFull(queue *q)
{
	return ((q->head +1) % q->capacity) == ((q->tail) % q->capacity);
}

bool enqueue(queue *q, items *i)
{
	if (queueFull(q))
	{
		return false;
	}
	else if (queueEmpty(q))
	{
		q->Q[q->head] = i;
		q->head += 1;
		q->head = q->head % q->capacity;
		q->size++;
		return true;
	}
	else
	{
		if( q->Q[q->head] != NULL)
		{
			delNode(q->Q[q->head]);
		}
		q->Q[q->head] = i;

		int j = q->head;
		while(
					j>=0
					&& (j-1)>=0
					&& q->Q[j] != NULL
					&& q->Q[j-1] != NULL
					&& q->Q[j-1]->count > q->Q[j]->count
				)
		{
			SWAP(q->Q[j-1], q->Q[j]);
			j--;
		}


		q->head +=1;
		q->head = q->head % q->capacity;
		q->size++;

		return true;

	}

}

bool dequeue(queue *q, items **i)
{
	if (queueEmpty(q))
	{
		return false;
	}
	else
	{
		if(i || !i)
		{
			*i = q->Q[q->tail];
		}
		q->tail = (q->tail + 1) % q->capacity;
		q->size--;
		return true;
	}

}
