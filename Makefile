OBJ_FILES := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
TESTER_FILES := $(patsubst tests/%.c, tests/obj/%.o, $(wildcard tests/*.c))
EXTRA_TESTER_FILES := $(patsubst tests/%Test.c, obj/%.o, $(wildcard tests/*Test.c))

CFLAGS += -O2 -ansi -Wall -Wextra -pedantic -Wunreachable-code \
                    -Wunused-parameter

gereVendas: $(OBJ_FILES)
	$(CC) -o $@ $^

tester: $(TESTER_FILES) $(EXTRA_TESTER_FILES)
	$(CC) -g -o $@ $^

debug: CFLAGS := -g
debug: clean gereVendas

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

tests/obj/%.o: tests/%.c
	@mkdir -p tests/obj
	$(CC) $(CFLAGS) -o $@ -c $<

obj/main.o: src/datacheck.h src/clients.h src/products.h src/generic.h
obj/datacheck.o: src/datacheck.h src/clients.h src/products.h src/generic.h src/sales.h 
obj/catalog.o: src/catalog.h src/avl.h src/generic.h
obj/avl.o: src/avl.h src/generic.h src/queue.h
obj/queue.o: src/queue.h
obj/clients.o: src/clients.h src/catalog.h src/generic.h
obj/products.o: src/products.h src/catalog.h src/generic.h
obj/sales.o: src/sales.h src/clients.h src/products.h src/generic.h
obj/interpreter.o: src/interpreter.h src/clients.h src/products.h

tests/obj/main.o: tests/avlTest.h tests/catalogTest.h 
tests/obj/catalogTest.o: tests/catalogTest.h obj/catalog.o
tests/obj/avlTest.o: tests/avlTest.h obj/avl.o obj/queue.o
tests/obj/revenueTest.o: tests/revenueTest.h obj/revenue.o


cleanAll: clean
	-@rm -rf doc

clean:
	-@rm -f gereVendas tester
	-@rm -f Vendas_1MValidas.txt
	-@rm -rf obj
	-@rm -rf tests/obj
	-@rm -f vg*

.PHONY: doc
doc:
	doxygen doxygen.conf
