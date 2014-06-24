TARGETS = stest vmtest kamvm
STACK := stack.c stack.h
VM := vm.c vm.h
CC ?= clang
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wnested-externs -Winline -Wno-long-long  -Wunused-variable
CFLAGS += -Wstrict-prototypes -Werror -ansi -static

.PHONY: all
all: $(TARGETS)

stest: $(STACK) stest.c
	$(CC) -o $@ stack.c stest.c

vmtest: $(STACK) $(VM) vmtest.c
	$(CC) -o $@ stack.c vm.c vmtest.c

kamvm: $(STACK) $(VM) kamvm.c
	$(CC) -o $@ stack.c vm.c kamvm.c

.PHONY: test
test: stest vmtest
	./stest && ./vmtest

.PHONY: clean
clean:
	rm -f *.o $(TARGETS)
