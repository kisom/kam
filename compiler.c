#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "queue.h"
#include "instr.h"


#define DEFAULT_BIN	(char *)"prog.bin"


struct s_compiler {
	queue8	q;
	int	state;
	uint8_t	buf[6];
	int	buf_index;
	uint8_t	n[2];
	FILE	*source;
	FILE	*out;
} compiler;


static void
init_compiler(void)
{
	compiler.q = new_queue8();
	if (!compiler.q)
		errx(EX_OSERR, "Failed to initialise compiler.");
	compiler.state = 0;
	memset(compiler.buf, 0, 6);
	compiler.buf_index = 0;
	memset(compiler.n, 0, 2);
	compiler.source = NULL;
	compiler.out = NULL;
}


static void
destroy_compiler(void)
{
	destroy_queue8(compiler.q);
	fclose(compiler.source);
	fclose(compiler.out);
}

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
	memcpy(compiler.n, &n16, 2);
	enqueue8(compiler.q, INSTR_IMM);
	enqueue8(compiler.q, compiler.n[0]);
	enqueue8(compiler.q, compiler.n[1]);
	memset(compiler.buf, 0, 6);
	memset(compiler.n, 0, 2);
	compiler.buf_index = 0;
	return 0;
}


static int
parse_next(uint8_t cur)
{
	uint8_t	instr;

	if (compiler.state) {
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
			compiler.state = 0;
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
		break;
	case '+':
		instr = INSTR_ADD;
		break;
	case '-':
		instr = INSTR_SUB;
		break;
	case '*':
		instr = INSTR_MUL;
		break;
	case '/':
		instr = INSTR_DIV;
		break;
	default:
		if ((cur > 0x2f) && (cur < 0x3a)) {
			compiler.state = 1;
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


static int
parse(void)
{
	uint8_t	cur;

	while (!feof(compiler.source)) {
		fread(&cur, 1, 1, compiler.source);
		if (-1 == parse_next(cur)) {
			return -1;		
		}
	}
	return 0;
}


static int
write_program(void)
{
	size_t	 prog_size;
	int	 pc = 0;
	uint8_t	*prog;

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
		fprintf(stderr, "Failed to write program to file.\n");
		return -1;
	}
	free(prog);

	return 0;
}


int
main(int argc, char *argv[])
{
	int	 opt, status = EXIT_SUCCESS;
	char	*prog_file = DEFAULT_BIN;

	init_compiler();

	while (-1 != (opt = getopt(argc, argv, "o:"))) {
		switch(opt) {
		case 'o':
			prog_file = optarg;
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
	}
	compiler.out = fopen(prog_file, "wb");
	if (NULL == compiler.out) {
		err(EX_IOERR, "failed to open output file.");
	}
	if (-1 == parse()) {
		fprintf(stderr, "Compilation failed.\n");
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
