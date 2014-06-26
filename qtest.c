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
