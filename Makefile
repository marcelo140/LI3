OBJ_FILES := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))

CFLAGS += -O2 -ansi -Wall -Wextra -pedantic -Wunreachable-code \
                    -Wunused-parameter

gereVendas: $(OBJ_FILES)
	$(CC) -o $@ $^

debug: CFLAGS := -g
debug: clear gereVendas

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<


obj/main.o: src/dataloader.h src/branchsale.h src/fatglobal.h src/clients.h src/products.h src/interpreter.h

obj/dataloader.o: src/dataloader.h src/fatglobal.h src/clients.h src/products.h src/generic.h src/sales.h src/branchsales.h

obj/catalog.o: src/catalog.h src/avl.h src/generic.h src/set.h

obj/avl.o: src/avl.h src/generic.h src/avl.h

obj/clients.o: src/clients.h src/catalog.h src/generic.h src/set.h
	
obj/products.o: src/products.h src/catalog.h src/generic.h src/set.h

obj/sales.o: src/sales.h src/clients.h src/products.h src/generic.h

obj/interpreter.o: src/interpreter.h src/clients.h src/products.h src/fatglobal.h src/branchsales.h src/dataloader.h src/queriesh

obj/fatglobal.o: src/sales.h src/generic.h src/fatglobal.h src/products.h src/catalog.h src/set.h

obj/branchsales.o: src/sales.h src/generic.h src/products.h src/clients.h src/catalog.h src/hashT.h src/branchsales.h

obj/set.o: src/generic.h src/set.h

obj/queris.o: src/set.h src/interpreter.h src/fatglobal.h src/branchsale.h

clearAll: clear
	-@rm -rf doc

.PHONY: clear
clear:
	-@rm -f gereVendas
	-@rm -rf obj
	-@rm -f vg*

.PHONY: doc
doc:
	doxygen doxygen.conf
