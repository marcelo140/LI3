C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS += -O2 -ansi -Wall

gereVendas: $(OBJ_FILES)
	$(CC) -o $@ $^

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

debug: CFLAGS := -g -ansi
debug: gereVendas

obj/main.o: src/datacheck.h
obj/datacheck.o: src/datacheck.h src/clients.h src/products.h
obj/catalog.o: src/catalog.h src/avl.h
obj/avl.o: src/avl.h
obj/clients.o: src/clients.h src/catalog.h
obj/products.o: src/products.h src/catalog.h

.PHONY: clean
clean:
	-@rm -f gereVendas *.o
	-@rm -f Vendas_1MValidas.txt
	-@rm -rf doc
	-@rm -rf obj

.PHONY: doc
doc:
	doxygen doxygen.conf
