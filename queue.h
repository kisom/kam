#ifndef __KAM_QUEUE_H
#define __KAM_QUEUE_H

#include <stdint.h>
#include "stack.h"

typedef struct s_queue8 {
	struct node8	*head;
	struct node8	*tail;
	size_t		 nodes;
} * queue8;

queue8	new_queue8(void);
void	enqueue8(queue8, uint8_t);
uint8_t	dequeue8(queue8);
int	empty_queue8(queue8);
void	destroy_queue8(queue8);


#endif
