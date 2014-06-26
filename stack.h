/*
 * Copyright (c) 2014 Kyle Isom <kyle@tyrfingr.is>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


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
