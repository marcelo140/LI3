OBJ_FILES := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))

CFLAGS += -O2 -ansi -Wall -Wextra -pedantic -Wunreachable-code \
                    -Wunused-parameter

gereVendas: $(OBJ_FILES)
	$(CC) -o $@ $^

tester: gereVendas
	$(MAKE) -C tests/

debug: CFLAGS := -g
debug: clear gereVendas

obj/%.o: src/%.c
	@mkdir -p obj
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
obj/fatglobal.o: src/sales.h src/fatglobal.h src/products.h src/catalog.h
obj/hashT.o: src/hashT.h src/sales.h src/products.h src/clients.h

clearAll: clean
	-@rm -rf doc

.PHONY: clear
clear: clearTests
	-@rm -f gereVendas
	-@rm -rf obj
	-@rm -f vg*

clearTests:
	-@$(MAKE) clear -C tests/

.PHONY: doc
doc:
	doxygen doxygen.conf
