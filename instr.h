#ifndef __KAM_INSTR_H
#define __KAM_INSTR_H


typedef enum e_op {
	INSTR_STOP	= 0,
	INSTR_ADD	= 1,
	INSTR_SUB	= 2,
	INSTR_MUL	= 3,
	INSTR_DIV	= 4,
	INSTR_IMM	= 5,
	INSTR_DO	= 6,
} op;


#endif
