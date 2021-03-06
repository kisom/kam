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


#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#include "vm.h"

static int
interpreter(void)
{
	fprintf(stderr, "The interpreter is not implemented yet.\n");
	return EXIT_FAILURE;
}


static int
run(const char *filespec)
{
	struct stat	 st;
	int		 status = EXIT_FAILURE;
	machine		 vm = NULL;
	FILE		*bin = NULL;
	uint8_t		*prog = NULL;

	if (-1 == stat(filespec, &st))
		goto exit;

	printf("Loading %lu byte program.\n", st.st_size);
	prog = malloc(st.st_size);
	if (NULL == prog)
		goto exit;

	if (NULL == (bin = fopen(filespec, "rb")))
		goto exit;

	if (st.st_size != (off_t)fread(prog, sizeof(uint8_t), st.st_size, bin))
		goto exit;

	vm = vm_new();
	if (NULL == vm)
		goto exit;

	printf("Starting VM.\n");
	if (VM_ERR == vm_run(vm, prog, st.st_size))
		goto exit;

	printf("> %d\n", vm_peek(vm));
	printf("OK\n");
	status = EXIT_SUCCESS;

exit:
	if (prog)
		free(prog);
	if (vm)
		vm_destroy(vm);
	if (bin)
		fclose(bin);
	return status;
}



int
main(int argc, const char *argv[])
{
	if (0 == argc)
		return interpreter();
	else
		return run(argv[1]);
}
