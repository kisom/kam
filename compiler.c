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


#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "queue.h"
#include "instr.h"


#define DEFAULT_BIN	(char *)"prog.bin"


/*
 * compiler contains the state for the compiler as it builds a
 * program.
 */
struct s_compiler {
	/*
	 * Instructions are pushed onto the queue, so that writing them
	 * later becomes a matter of just dequeuing instructions off
	 * the queue and into a buffer.
	 */
	queue8	q;

	/*
	 * in_num is 1 when a number is being processed.
	 */
	int	in_num;

	/*
	 * buf contains the buffer for reading in integers, as this is
	 * processed character by character. Once the compiler transitions
	 * form in_num=1 to in_num=0, this will be converted to an
	 * integer. The n buffer is used to write the two bytes of the
	 * 16-bit integer.
	 */
	uint8_t	buf[6];
	int	buf_index;

	/*
	 * Every time an operation is encountered, the active_ops counter
	   is incremented; similarly, every time a value is encountered,
	   the active_vals counter is incremented. Both of these are
	   decremented every time a DO is encountered; a DO takes an
	   operator and a pair of values, applies the operator to the
	   values, and pushes the result back onto the stack, so only
	   one decrement is needed for a DO. Once parsing is complete,
	   the compiler checks to ensure there is one remaining value
	   (the final result) and no operations remaining. This provides
	   rudimentary syntax checking.
	   */
	int	active_ops;
	int	active_vals;

	/*
	 * source is the file stream being read from, while out is the
	 * output file stream.
	 */
	FILE	*source;
	FILE	*out;
} compiler;


/*
 * init_compiler allocates a new queue for the compiler and ensures
 * everything else is initialised with a zero value.
 */
static void
init_compiler(void)
{
	compiler.q = new_queue8();
	if (!compiler.q)
		errx(EX_OSERR, "Failed to initialise compiler.");
	compiler.in_num = 0;
	memset(compiler.buf, 0, 6);
	compiler.buf_index = 0;
	compiler.active_ops = 0;
	compiler.active_vals = 0;
	compiler.source = NULL;
	compiler.out = NULL;
}


/*
 * destroy_compiler frees the queue and closes the source and out streams.
 */
static void
destroy_compiler(void)
{
	destroy_queue8(compiler.q);
	fclose(compiler.source);
	fclose(compiler.out);
}


/*
 * finalise_number converts the number that has been stored in the
 * compiler's numeric buffer to a 32-bit integer, which is is checked
 * for integer overflow. The result is then pushed onto the queue with
 * the appropriate leading IMM instruction.
 */
static int
finalise_number(void)
{
	uint32_t	n32;
	uint16_t	n16;

	n32 = (uint32_t)atoi((char *)compiler.buf);
	if (n32 >= (1 << 17)) {
		fprintf(stderr, "Integer overflow: %lu\n",
		    (long unsigned)n32);
		return -1;
	}

	n16 = (uint16_t)n32;
	enqueue8(compiler.q, INSTR_IMM);
	enqueue8(compiler.q, (uint8_t)(n16<<8>>8));
	enqueue8(compiler.q, (uint8_t)(n16>>8));
	memset(compiler.buf, 0, 6);
	compiler.buf_index = 0;
	compiler.active_vals++;
	return 0;
}


/*
 * parse_next deals with the next byte read from the input stream. It
 * handles storing numbers using the internal numeric buffer, finalising
 * numbers and continuing to parse the instruction. It also performs
 * checks on the balance of active operations and values.
 */
static int
parse_next(uint8_t cur)
{
	uint8_t	instr;

	if (compiler.in_num) {
		if ((cur > 0x2f) && (cur < 0x3a)) {
			if (compiler.buf_index == 5) {
				fprintf(stderr, "Compile failed: overflow.\n");
				return -1;
			}
			compiler.buf[compiler.buf_index++] = cur;
			return 0;
		} else {
			if (-1 == finalise_number()) {
				return -1;
			}
			compiler.in_num = 0;
		}
	}

	switch (cur) {
	case ' ':
	case '\t':
	case '\n':
	case '(':
		return 0;
	case ')':
		instr = INSTR_DO;
		if (0 == compiler.active_ops) {
			fprintf(stderr, "No active operators for DO.\n");
			return -1;
		}
		compiler.active_ops--;
		if (compiler.active_vals < 2) {
			fprintf(stderr, "Not enough values (%d) for DO.\n",
			    compiler.active_vals);
			return -1;
		}
		compiler.active_vals--;
		break;
	case '+':
		compiler.active_ops++;
		instr = INSTR_ADD;
		break;
	case '-':
		compiler.active_ops++;
		instr = INSTR_SUB;
		break;
	case '*':
		compiler.active_ops++;
		instr = INSTR_MUL;
		break;
	case '/':
		compiler.active_ops++;
		instr = INSTR_DIV;
		break;
	default:
		if ((cur > 0x2f) && (cur < 0x3a)) {
			compiler.in_num = 1;
			compiler.buf[compiler.buf_index++] = cur;
			return 0;
		} else {
			fprintf(stderr, "Unrecognised token %d\n", cur);
			return -1;
		}
	}
	enqueue8(compiler.q, instr);
	return 0;
}


/*
 * parse reads input from the source file and sends it to be compiled
 * compiled. Once compilation is complete, the value and operator
 * balance is checked.
 */
static int
parse(void)
{
	uint8_t	cur;

	assert(NULL != compiler.source);

	while (!feof(compiler.source)) {
		fread(&cur, 1, 1, compiler.source);
		if (-1 == parse_next(cur)) {
			return -1;
		}
	}

	if (1 != compiler.active_vals) {
		fprintf(stderr, "Warning: extra values left on the stack.\n");
	}

	if (0 != compiler.active_ops) {
		fprintf(stderr, "Warning: active operations remaining.\n");
	}
	return 0;
}


/*
 * write_program copies values off the queue into the output file.
 */
static int
write_program(void)
{
	size_t	 prog_size;
	int	 pc = 0;
	uint8_t	*prog;

	assert(NULL != compiler.out);

	prog_size = compiler.q->nodes;
	if (0 == prog_size) {
		printf("Empty program.\n");
		return -1;
	}
	printf("Compile size: %lu bytes\n", prog_size);

	prog = malloc(prog_size);
	if (NULL == prog)
		errx(EX_OSERR, "Failed to initialise memory.");

	while (!empty_queue8(compiler.q))
		prog[pc++] = dequeue8(compiler.q);

	if (0 == fwrite(prog, 1, prog_size, compiler.out)) {
		free(prog);
		fprintf(stderr, "Failed to write program to file.\n");
		return -1;
	}
	free(prog);

	return 0;
}


/*
 * backtrace shows the currently compiled program and the compiler's
 * state.
 */
static void
backtrace(int trace)
{
	int i;
	int wrap = 0;

	if (!trace)
		return;

	printf("in_num: %d\n", compiler.in_num);
	printf("n_buf (%dB): ", compiler.buf_index);
	for (i = 0; i < 6; i++) {
		printf("%02x ", compiler.buf[i]);
	}
	printf("\n");
	printf("active\tvals: %d\tops: %d\n", compiler.active_vals, compiler.active_ops);

	printf("Compiled:\n\t");
	while (!empty_queue8(compiler.q)) {
		printf("%02x ", dequeue8(compiler.q));
		if (8 == (++wrap)) {
			printf("\n\t");
			wrap = 0;
		}
	}
	printf("\n");
}


/*
 * kamc is the compiler for KAM. It emits binary programs that run on
 * the KAM virtual machine.
 */
int
main(int argc, char *argv[])
{
	int	 opt, trace, status = EXIT_SUCCESS;
	char	*prog_file = DEFAULT_BIN;

	init_compiler();

	while (-1 != (opt = getopt(argc, argv, "o:t"))) {
		switch(opt) {
		case 'o':
			prog_file = optarg;
			break;
		case 't':
			trace = 1;
			break;
		default:
			/* not reached */
			abort();
		}
	}
	argc -= optind;
	argv += optind;

	if (0 == argc) {
		compiler.source = stdin;
	} else if (1 == argc) {
		printf("Source file: %s\n", argv[0]);
		compiler.source = fopen(argv[0], "rb");
		if (NULL == compiler.source) {
			err(EX_IOERR, "failed to open source file.");
		}
	} else {
		fprintf(stderr, "Compiling multiple files is not supported.\n");
		exit(EX_USAGE);
	}
	compiler.out = fopen(prog_file, "wb");
	if (NULL == compiler.out) {
		err(EX_IOERR, "failed to open output file.");
	}
	if (-1 == parse()) {
		fprintf(stderr, "Compilation failed.\n");
		backtrace(trace);
		status = EXIT_FAILURE;
		goto exit;
	} else if (-1 == write_program()) {
		fprintf(stderr, "Writing binary failed.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	printf("Wrote compiled program to %s.\n", prog_file);

exit:
	destroy_compiler();
	return status;
}
