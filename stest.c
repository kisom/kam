#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>

#include "stack.h"


static void
test_stack8(void)
{
	uint8_t val;
	stack8	st;

	st = new_stack8();
	if (st == NULL) {
		errx(EX_OSERR, "FAILED: malloc failure");
	}

	push8(st, 1);
	push8(st, 2);
	if (2 != (val = pop8(st))) {
		fprintf(stderr,
		    "FAILED: should have received 2 from the stack.\n");
		fprintf(stderr,
		    "        (received %d)\n", val);
		exit(EXIT_FAILURE);
	}

	push8(st, 3);
	if (3 != (val = pop8(st))) {
		fprintf(stderr,
		    "FAILED: should have received 3 from the stack.\n");
		fprintf(stderr,
		    "        (received %d)\n", val);
		exit(EXIT_FAILURE);
	}
	if (1 != (val = pop8(st)))
		errx(EX_SOFTWARE,
		    "FAILED: should have received 1 from the stack.");
	if (!empty8(st))
		errx(EX_SOFTWARE,
		    "FAILED: stack should report empty.");
	push8(st, 5);
	push8(st, 5);
	destroy8(st);
	printf("stack8: OK\n");
}


static void
test_stack16(void)
{
	uint16_t val;
	stack16	st;

	st = new_stack16();
	if (st == NULL) {
		errx(EX_OSERR, "FAILED: malloc failure");
	}

	push16(st, 1);
	push16(st, 2);
	if (2 != (val = pop16(st))) {
		fprintf(stderr,
		    "FAILED: should have received 2 from the stack.\n");
		fprintf(stderr,
		    "        (received %d)\n", val);
		exit(EXIT_FAILURE);
	}

	push16(st, 3);
	if (3 != (val = pop16(st))) {
		fprintf(stderr,
		    "FAILED: should have received 3 from the stack.\n");
		fprintf(stderr,
		    "        (received %d)\n", val);
		exit(EXIT_FAILURE);
	}
	if (1 != pop16(st))
		errx(EX_SOFTWARE,
		    "FAILED: should have received 1 from the stack.");
	if (!empty16(st))
		errx(EX_SOFTWARE,
		    "FAILED: stack should report empty.");

	for (val = 0; val < 16000; val++)
		push16(st, val);

	destroy16(st);
	printf("stack16: OK\n");
}


int
main(void)
{
	test_stack8();
	test_stack16();

	return EXIT_SUCCESS;
}
