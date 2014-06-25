#ifndef __KAM_LIST_H
#define __KAM_LIST_H

#include <sys/types.h>
#include <stdint.h>


#include "node.h"


typedef struct s_stack8 {
	struct node8	*st;
} * stack8;

stack8	new_stack8(void);
void	push8(stack8, uint8_t);
uint8_t	pop8(stack8);
int	empty8(stack8);
void	destroy8(stack8);


struct node16 {
	uint16_t	 val;
	struct node16	*next;
};
typedef struct s_stack16 {
	struct node16	*st;
} * stack16;

stack16		new_stack16(void);
void		push16(stack16, uint16_t);
uint16_t	pop16(stack16);
int		empty16(stack16);
void		destroy16(stack16);

#endif
