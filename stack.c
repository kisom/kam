#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>


#include "node.h"
#include "stack.h"


/*
 * Allocate a new 8-bit stack.
 */
stack8
new_stack8()
{
	stack8 st = NULL;

	st = malloc(sizeof(struct s_stack8));
	if (NULL == st)
		return NULL;
	st->st = NULL;
	return st;
}


/*
 * push8 pushes a new value onto an 8-bit stack.
 */
void
push8(stack8 st, uint8_t val)
{
	struct node8	*node;

	node = malloc(sizeof(struct node8));

	/* memory problems can only lead to more memory problems. */
	if (NULL == node)
		abort();

	node->val = val;
	node->next = st->st;
	st->st = node;
}


/*
 * Remove the first value from the stack and return it.
 */
uint8_t
pop8(stack8 st)
{
	uint8_t	val;
	struct node8 *node;

	if (NULL == st->st)
		abort();

	node = st->st;
	st->st = st->st->next;

	val = node->val;
	free(node);
	return val;
}


int
empty8(stack8 st)
{
	return st->st == NULL;
}


void
destroy8(stack8 st)
{
	struct node8 *node = NULL;

	while (NULL != (node = st->st)) {
		st->st = node->next;
		free(node);
	}
	free(st);
}


/*
 * Allocate a new 16-bit stack.
 */
stack16
new_stack16()
{
	stack16 st = NULL;

	st = malloc(sizeof(struct s_stack16));
	if (NULL == st)
		return NULL;
	st->st = NULL;
	return st;
}


/*
 * push16 pushes a new value onto an 16-bit stack.
 */
void
push16(stack16 st, uint16_t val)
{
	struct node16	*node;

	node = malloc(sizeof(struct node16));

	/* memory problems can only lead to more memory problems. */
	if (NULL == node)
		abort();

	node->val = val;
	node->next = st->st;
	st->st = node;
}


/*
 * Remove the first value from the stack and return it.
 */
uint16_t
pop16(stack16 st)
{
	uint16_t	val;
	struct node16 *node;

	if (NULL == st->st)
		abort();

	node = st->st;
	st->st = st->st->next;

	val = node->val;
	free(node);
	return val;
}


int
empty16(stack16 st)
{
	return st->st == NULL;
}


void
destroy16(stack16 st)
{
	struct node16 *node = NULL;

	while (NULL != (node = st->st)) {
		st->st = node->next;
		free(node);
	}
	free(st);
}
