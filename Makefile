TARGETS = stest qtest vmtest kamvm kamc
STACK := stack.c stack.h node.h
QUEUE := queue.c queue.h node.h
COMPILER := compiler.c
VM := vm.c vm.h
CC ?= clang
LEX ?= flex
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wnested-externs -Winline -Wno-long-long  -Wunused-variable
CFLAGS += -Wstrict-prototypes -Werror -ansi -static -D_XOPEN_SOURCE
CFLAGS += -g -O0

.PHONY: all
all: $(TARGETS)

stest: $(STACK) stest.c
	$(CC) $(CFLAGS) -o $@ stack.c stest.c

qtest: $(QUEUE) qtest.c
	$(CC) $(CFLAGS) -o $@ queue.c stack.c qtest.c

vmtest: $(STACK) $(VM) vmtest.c
	$(CC) $(CFLAGS) -o $@ stack.c vm.c vmtest.c

kamvm: $(STACK) $(VM) kamvm.c
	$(CC) $(CFLAGS) -o $@ stack.c vm.c kamvm.c

kamc: $(QUEUE) compiler.c
	$(CC) $(CFLAGS) -o $@ stack.c queue.c compiler.c

.PHONY: test
test: stest qtest vmtest kamc kamvm
	./stest && ./qtest && ./vmtest && ./test_compiler.sh \
		&& ./test_vm.sh && echo "All tests passed."

.PHONY: clean
clean:
	rm -f *.o $(TARGETS) core prog.bin
