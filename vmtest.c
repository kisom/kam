#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>

#include "vm.h"


static void
test1(void)
{
	int		vm_status;
	/*
	 * (5 + (2 * 2))
	 * program:
	 * 05 05 00	; IMM 0x0005
	 * 05 02 00	; IMM 0x0002
	 * 05 02 00	; IMM 0x0002
	 * 03		; MUL
	 * 01		; ADD
	 * 06		; DO (ADD 0x0002 0x0002)
	 * 06		; DO (MUL 0x0004 0x0005)
	 */
	uint8_t		prog[] = "\x05\x05\x00\x03\x05\x02\x00\x01\x05"
	    			 "\x02\x00\x06\x06";
	uint16_t	prog_len = sizeof(prog)+1;
	machine		vm;

	vm = vm_new();
	if (NULL == vm)
		errx(EX_OSERR, "FAILED: memory allocation error.\n");

	vm_status = vm_run(vm, prog, prog_len);
	if (VM_ERR == vm_status) {
		errx(EX_SOFTWARE, "FAILED: VM stopped with error.\n");
	}

	printf("> %d\n", vm_peek(vm));
	if (vm_peek(vm) != 20)
		fprintf(stderr, "prog2.bin: FAILED\n");
	else
		printf("prog1.bin: OK\n");

	vm_destroy(vm);
}


static void
test2(void)
{
	int		vm_status;
	/*
	 * program:
	 * (2 + (3 * (4 / 2))) ; = 8
	 *
	 * 05 02 00	; IMM 0x0002
	 * 01		; ADD
	 * 05 03 00	; IMM 0x0003
	 * 03		; MUL
	 * 05 04 00	; IMM 0x0004
	 * 04		; DIV
	 * 05 02 00	; IMM 0x0002
	 * 06		; DO (DIV 0x0004 0x0002)
	 * 06		; DO (MUL 0x0003 0x0002)
	 * 06		; DO (MUL 0x0002 0x0006)
	 */
	uint8_t		prog[] = "\x05\x02\x00\x01\x05\x03\x00\x03"
				 "\x05\x04\x00\x04\x05\x02\x00\x06"
				 "\x06\x06";
	uint16_t	prog_len = sizeof(prog);
	machine		vm;

	vm = vm_new();
	if (NULL == vm)
		errx(EX_OSERR, "FAILED: memory allocation error.\n");

	vm_status = vm_run(vm, prog, prog_len);
	if (VM_ERR == vm_status) {
		errx(EX_SOFTWARE, "FAILED: VM stopped with error.\n");
	}

	printf("> %d\n", vm_peek(vm));
	if (vm_peek(vm) != 8)
		fprintf(stderr, "prog2.bin: FAILED\n");
	else
		printf("prog2.bin: OK\n");

	vm_destroy(vm);
}


int
main(void)
{
	printf("Starting VM tests.\n");
	test1();
	test2();
	printf("VM tests complete.\n");

	return EXIT_SUCCESS;
}
