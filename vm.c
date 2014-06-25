#include <sys/types.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instr.h"
#include "stack.h"
#include "vm.h"

/*
 * Create a new virtual machine.
 */
machine
vm_new(void)
{
	machine vm = NULL;

	vm = malloc(sizeof(struct s_vm));
	if (NULL == vm)
		return NULL;

	vm->vals = new_stack16();
	vm->opers = new_stack8();
	vm->pc = 0;

	if ((NULL == vm->vals) || (NULL == vm->opers)) {
		vm_destroy(vm);
		return NULL;
	}
	return vm;
}


/*
 * Destroy a virtual machine and reclaim its resources.
 */
void
vm_destroy(machine vm)
{
	if (vm) {
		if (vm->vals)
			destroy16(vm->vals);
		if (vm->opers)
			destroy8(vm->opers);
		free(vm);
	}
}


/*
 * eval is an internal function that executes operations when a DO
 * instruction is encountered.
 */
static int
eval(machine vm)
{
	uint16_t a, b;
	uint8_t	oper;

	b = pop16(vm->vals);
	a = pop16(vm->vals);
	oper = pop8(vm->opers);

	switch (oper) {
	case INSTR_ADD:
		push16(vm->vals, a + b);
		break;
	case INSTR_SUB:
		push16(vm->vals, a - b);
		break;
	case INSTR_MUL:
		push16(vm->vals, a * b);
		break;
	case INSTR_DIV:
		push16(vm->vals, a / b);
		break;
	default:
		abort(); /* illegal instruction trap ;) */
	}
	return VM_OK;
}


/*
 * Step through a single instruction in the program. Returns VM_ERR
 * on an error condition (if it doesn't abort()), VM_STOP if the
 * program is complete, and VM_OK if the program should keep running.
 */
int
vm_step(machine vm, uint8_t *prog, uint16_t prog_len)
{
	uint16_t	val;
	uint8_t		oper;

	if (NULL == vm) {
		fprintf(stderr, "[!] VM not initialised.\n");
		return VM_ERR;
	}

	if (vm->pc == prog_len)
		return VM_STOP;

	oper = prog[vm->pc];
	vm->pc++;
	switch (oper) {
	case INSTR_STOP:
		return VM_STOP;
	case INSTR_ADD:  /* fall through: store the operator in */
	case INSTR_SUB:  /* the op stack                        */
	case INSTR_DIV:
	case INSTR_MUL:
		push8(vm->opers, oper);
		break;
	case INSTR_IMM:
		/* detect wrap around */
		if ((prog_len - vm->pc) >= prog_len) {
			fprintf(stderr, "[!] PC overflow.\n");
			return VM_ERR;
		}
		/* ensure we have enough progspace left */
		if ((prog_len - vm->pc) < 2) {
			fprintf(stderr, "[!] Out of program space.\n");
			fprintf(stderr, "    %d bytes remain.\n", 
			    prog_len - vm->pc);
			return VM_ERR;
		}
		memcpy(&val, prog + vm->pc, sizeof(val));
		push16(vm->vals, val);
		vm->pc += 2; /* 2 bytes == 16 bits */
		break;
	case INSTR_DO:
		return eval(vm);
	default:
		fprintf(stderr, "[!] Unknown operator: %d.\n", oper);
		return VM_ERR;

	}
	return VM_OK;
}


int
vm_run(machine vm, uint8_t *prog, uint16_t prog_len)
{
	int	vm_status;

	do {
		vm_status = vm_step(vm, prog, prog_len);

	} while (VM_OK == vm_status);

	return vm_status;
}


uint16_t
vm_peek(machine vm)
{
	uint16_t val;

	if (NULL == vm)
		return 0;
	if (NULL == vm->vals)
		return 0;
	if (empty16(vm->vals))
		return 0;

	val = pop16(vm->vals);
	push16(vm->vals, val);
	return val;
}

