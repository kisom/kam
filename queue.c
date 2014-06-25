#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>

#include "node.h"
#include "queue.h"

queue8
new_queue8()
{
	queue8	q;

	q = malloc(sizeof(struct s_queue8));
	if (NULL == q)
		abort();
	q->head = NULL;
	q->tail = NULL;
	q->nodes = 0;
	return q;
}

void
enqueue8(queue8 q, uint8_t val)
{
	struct node8	*node;

	node = malloc(sizeof(struct node8));

	/* memory problems can only lead to more memory problems. */
	if (NULL == node)
		abort();

	node->val = val;

	/*
	 * If queue is empty, head will be NULL and tail will be NULL.
	 * In this case, we assign both to node.
	 */
	if (NULL == q->head) {
		if (NULL != q->tail) {
			abort();
		}
		q->head = node;
		q->tail = node;
		node->next = NULL;
	} else {
		node->next = NULL;
		q->tail->next = node;
		q->tail = node;
	}
	q->nodes++;
}


uint8_t
dequeue8(queue8 q)
{
	struct node8	*node;
	uint8_t	val;

	if (NULL == q->head)
		abort();

	node = q->head;
	val = node->val;

	q->head = node->next;
	free(node);
	q->nodes--;
	if (0 == q->nodes)
		q->tail = NULL;
	return val;
}


int
empty_queue8(queue8 q)
{
	return q->nodes == 0;
}


void
destroy_queue8(queue8 q)
{
	while (!empty_queue8(q))
		dequeue8(q);
	free(q);
}
