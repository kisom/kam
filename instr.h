#ifndef __KAM_INSTR_H
#define __KAM_INSTR_H


typedef enum e_op {
	INSTR_ADD	= 1,
	INSTR_SUB	= 2,
	INSTR_MUL	= 3,
	INSTR_DIV	= 4,
	INSTR_IMM	= 5,
	INSTR_DO	= 6,
	INSTR_EQL	= 7,
	INSTR_JRE	= 8,
	INSTR_JRN	= 9,
	INSTR_JE	= 10,
	INSTR_JN	= 11
} op;


#endif
