#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>

#include "queue.h"


static void
test_queue8(void)
{
	uint8_t val;
	queue8	st;

	st = new_queue8();
	if (st == NULL) {
		errx(EX_OSERR, "FAILED: malloc failure");
	}

	enqueue8(st, 1);
	enqueue8(st, 2);
	if (1 != (val = dequeue8(st))) {
		fprintf(stderr,
		    "FAILED: should have received 2 from the queue.\n");
		fprintf(stderr,
		    "        (received %d)\n", val);
		exit(EXIT_FAILURE);
	}

	enqueue8(st, 3);
	if (2 != (val = dequeue8(st))) {
		fprintf(stderr,
		    "FAILED: should have received 3 from the queue.\n");
		fprintf(stderr,
		    "        (received %d)\n", val);
		exit(EXIT_FAILURE);
	}
	if (3 != (val = dequeue8(st)))
		errx(EX_SOFTWARE,
		    "FAILED: should have received 1 from the queue.");
	if (!empty_queue8(st))
		errx(EX_SOFTWARE,
		    "FAILED: queue should report empty.");
	enqueue8(st, 5);
	enqueue8(st, 5);
	destroy_queue8(st);
	printf("queue8: OK\n");
}


int
main(void)
{
	test_queue8();

	return EXIT_SUCCESS;
}
