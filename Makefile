TARGETS = stest vmtest kamvm compiler
STACK := stack.c stack.h
COMPILER := compiler.c compiler.h compiler.yy.c
VM := vm.c vm.h
CC ?= clang
LEX ?= flex
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wnested-externs -Winline -Wno-long-long  -Wunused-variable
CFLAGS += -Wstrict-prototypes -Werror -ansi -static -g -O0

.PHONY: all
all: $(TARGETS)

stest: $(STACK) stest.c
	$(CC) $(CFLAGS) -o $@ stack.c stest.c

vmtest: $(STACK) $(VM) vmtest.c
	$(CC) $(CFLAGS) -o $@ stack.c vm.c vmtest.c

kamvm: $(STACK) $(VM) kamvm.c
	$(CC) $(CFLAGS) -o $@ stack.c vm.c kamvm.c

jmpcalc: jmpcalc.c
	$(CC) $(CFLAGS) -o $@ jmpcalc.c

compiler: compiler.l compiler.y
	$(YACC) -o compiler.c -d compiler.y
	$(LEX) -o compiler.yy.c compiler.l
	$(CC) -o $@ compiler.c compiler.yy.c -lfl

.PHONY: test
test: stest vmtest
	./stest && ./vmtest

.PHONY: clean
clean:
	rm -f *.o $(TARGETS) core $(COMPILER)
