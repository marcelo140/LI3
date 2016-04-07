OBJ_FILES := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
TESTER_FILES := $(patsubst tests/%.c, tests/obj/%.o, $(wildcard tests/*.c))

CFLAGS += -O2 -ansi -Wall -Wextra -pedantic -Wunreachable-code \
                    -Wunused-parameter

gereVendas: $(OBJ_FILES)
	$(CC) -o $@ $^

tester: $(TESTER_FILES)
	$(CC) -o $@ $^

debug: CFLAGS := -g
debug: clean gereVendas

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

test/obj/$.o: tests/%.c
	@mkdir -p test/obj
	@(CC) -O2 -o $@ -c $<

obj/main.o: src/datacheck.h src/clients.h src/products.h src/generic.h
obj/datacheck.o: src/datacheck.h src/clients.h src/products.h src/generic.h src/sales.h 
obj/catalog.o: src/catalog.h src/avl.h src/generic.h
obj/avl.o: src/avl.h src/generic.h src/queue.h
obj/queue.o: src/queue.h
obj/clients.o: src/clients.h src/catalog.h src/generic.h
obj/products.o: src/products.h src/catalog.h src/generic.h
obj/sales.o: src/sales.h src/clients.h src/products.h src/generic.h

tests/obj/main.o: tests/avlTest.h tests/catTest.h 
tests/obj/catTest.o: tests/catTest.h src/catalog.h
tests/obj/avlTest.o: tests/avlTest.h src/avl.h


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
