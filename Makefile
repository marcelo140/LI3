OBJ_FILES := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
CFLAGS += -O2 -ansi -Wall -Wextra -pedantic -Wunreachable-code \
                    -Wmissing-prototypes -Wunused-parameter

gereVendas: $(OBJ_FILES)
	$(CC) -o $@ $^

debug: CFLAGS := -g
debug: gereVendas

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

obj/main.o: src/datacheck.h src/generic.h
obj/datacheck.o: src/datacheck.h src/clients.h src/products.h src/generic.h
obj/catalog.o: src/catalog.h src/avl.h src/generic.h
obj/avl.o: src/avl.h src/generic.h
obj/clients.o: src/clients.h src/catalog.h src/generic.h
obj/products.o: src/products.h src/catalog.h src/generic.h

cleanAll: clean
	-@rm -rf doc

clean:
	-@rm -f gereVendas *.o
	-@rm -f Vendas_1MValidas.txt
	-@rm -rf obj

.PHONY: doc
doc:
	doxygen doxygen.conf
