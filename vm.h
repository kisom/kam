#ifndef __KAM_VM_H
#define __KAM_VM_H

#include "stack.h"

#define VM_ERR	0
#define VM_OK	1
#define VM_STOP 2


typedef struct s_vm {
	stack16		vals;
	stack8		opers;
	uint16_t	pc;
} * machine;


machine		vm_new(void);
void		vm_destroy(machine);
int		vm_step(machine, uint8_t *, uint16_t);
int		vm_run(machine, uint8_t *, uint16_t);
uint16_t	vm_peek(machine);



#endif
